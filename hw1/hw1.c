#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 0.001


/*
datapoints are stored like so:
for ease of display, think of all the datapoints as a matrix A whose rows are datapoints.
the dimension of the data (how many values per datapoint) is m, and the number of datapoints is n.


IN PYTHON:

(
    (A_11, A_12, ..., A_1m),
    (A_21, A_22, ..., A_2m),
    ...,
    (A_n1, A_n2, ..., A_nm)
)


IN C (which is here):

DPE_ij = DPE(
    A_ij,         // value
    DPE_i(j+1)    // next_entry, go right to the next coordinate on the datapoint
    DPE_(i+1)j    // next_dp, go down to the same coordinate on the next datapoint
)

on the final line/column,
the pointers point to NULL,
which is the indicator that it is the last DPE on the line/column

*/



struct DPE {  /*datapoint entry*/
    double value;
    struct DPE *next_entry;
    struct DPE *next_dp;
};
typedef struct DPE DPE;


struct Cluster {
    DPE *first_vector_head;
    DPE *last_vector_head;
    struct Cluster *next_cluster;
};
typedef struct Cluster Cluster;


DPE* create_vector(int d, DPE *prev_vec, DPE *original_vec) {
    /*
    creates a vector of DPEs with length d.
    iterates for d-1 steps as we already have the head of the vector.
    if at some point the allocation returns NULL, the function returns NULL because the creation failed.
    if prev_vec is not NULL, assigns the created vector to be the next vector relative to prev_vec.
    if original_vec is not NULL, copies original_vec's values into the new vector.
    */
    
    DPE *head = malloc(sizeof(DPE));
    DPE *dpe = head;
    
    int i;

    if (dpe == NULL)
        exit(1);
    
    /*
    handles the head of the vectos first.
    */
    
    if (original_vec != NULL) {
        dpe->value = original_vec->value;
        original_vec = original_vec->next_entry;
    }
    
    if (prev_vec != NULL) {
        prev_vec->next_dp = dpe;
        prev_vec = prev_vec->next_entry;
    }
    
    /*
    then all the entries that follow.
    */

    for (i = 1; i < d; i++) {
        dpe->next_entry = malloc(sizeof(DPE));
        dpe = dpe->next_entry;

        if (dpe == NULL)
            exit(1);
        
        if (original_vec != NULL) {
            dpe->value = original_vec->value;
            original_vec = original_vec->next_entry;
        }

        if (prev_vec != NULL) {
            prev_vec->next_dp = dpe;
            prev_vec = prev_vec->next_entry;
        }
    }
    
    return head;
}


void free_dpe_matrix(DPE *vec_head) {
    /*
    frees the entire matrix which is accessible from vec_head.

    vec_head is the head of the first vector in the matrix.
    */
    
    DPE *dpe, *next_dpe, *next_vec_head;

    while (vec_head != NULL) {
        dpe = vec_head;
        next_vec_head = vec_head->next_dp;

        while (dpe != NULL) {
            next_dpe = dpe->next_entry;
            free(dpe);
            dpe = next_dpe;
        }

        vec_head = next_vec_head;
    }
}


void free_clusters(Cluster *cluster) {
    /*
    frees the entire clusters which are accessible from cluster.

    cluster is the first cluster.
    */

    Cluster *next_cluster;

    while (cluster != NULL) {
        next_cluster = cluster->next_cluster;

        free_dpe_matrix(cluster->first_vector_head);
        free(cluster);

        cluster = next_cluster;
    }
}


double distance(DPE *vec1, DPE *vec2, int d) {
    /*
    calculates the distance of vec1 from vec2 based on the Pythagorean theorem (see pdf for definition).

    vec1 and vec2 are the first coordinates of datapoints, like A_i1, A_j1 in the matrix.
    d is the length of vec1 and vec2.
    */

    double sum = 0.0, diff;
    int i;

    for (i = 0; i < d; i++) {
        /*
        iterate over all the entries of the vectors
        */

        diff = vec1->value - vec2->value;
        sum += diff * diff;

        vec1 = vec1->next_entry;
        vec2 = vec2->next_entry;
    }
    
    return sqrt(sum);
}


void add_vec_to_closest_cluster(DPE *vec, DPE *centroid, Cluster *cluster, int d, int k) {
    /*
    finds the closest centroid to vec, and appends it to the corresponding cluster.

    vec is the head of a vector,
    centroid is the head of the first centroid.
    cluster is a pointer to the first cluster.
    d is the length of all the vectors.
    k is the number of centroids/clusters.
    */

    int min_index = 0, i = 0;
    double min_distance = -1;

    for (i = 0; i < k; i++) {
        /*
        iterates over all vectors in centroids.
        for each one, calculates its distance from vec, and saves the minimal index.
        */

        if ((distance(vec, centroid, d) < min_distance) || (min_distance == -1)) {
            min_distance = distance(vec, centroid, d);
            min_index = i;
        }

        centroid = centroid->next_dp;
    }
    
    for (i = 0; i < min_index; i++) {
        /*
        finds the correct cluster based on min_index
        */
        
        cluster = cluster->next_cluster;
    }
    
    cluster->last_vector_head = create_vector(d, cluster->last_vector_head, vec);
    
    if (cluster->first_vector_head == NULL)
        cluster->first_vector_head = cluster->last_vector_head;
}


DPE* update_centroid(DPE *cluster_first_vec, int d, DPE *prev_centroid) {
    /*
    based on a (linked) list of datapoints, calculates their centroid using the mean vector (see pdf for definition).

    cluster_first_vec is the first coordinate of the first datapoint of the cluster, like A_11.
    d is the length of the vectors.
    prev_centroid is the head of the previous centroid.
    not all the datapoints from the file are proccessed here, only a certain cluster.
    */

    double sum = 0.0;
    int i, num_of_vectors = 0;

    DPE *cluster_dpe = cluster_first_vec;

    DPE *new_centroid_head = create_vector(d, prev_centroid, NULL);
    DPE *new_centroid_dpe = new_centroid_head;

    for (i = 0; i < d; i++) {
        /*
        iterates over the coordiantes of the clusters and then iterate over the datapoinsts.
        the idea is to calculate each coordiante of the resulting vector by
        going over each the same coordinate of every datapoint.
        */

        cluster_dpe = cluster_first_vec;
        sum = 0.0;
        num_of_vectors = 0;

        while (cluster_dpe != NULL) {
            /*
            iterates over all datapoints in the cluster, for the same coordinate,
            and save the mean into the value of the next coordiante in the new centroid.
            */

            sum += cluster_dpe->value;
            num_of_vectors++;
            cluster_dpe = cluster_dpe->next_dp;
        }

        new_centroid_dpe->value = sum / num_of_vectors;

        cluster_first_vec = cluster_first_vec->next_entry;
        new_centroid_dpe = new_centroid_dpe->next_entry;
    }

    return new_centroid_head;
}


DPE* iteration(DPE *vec, DPE *centroids, int d, int k) {
    /*
    first, creates a linked-list of the new cenetroids.
    then, for each vector in the given datapoints,
    calls add_vec_to_closest_cluster and appends it to its cluster, based on the closest centroid.
    finally, updates the first centroid and then all the following centroids.
    because we don't need to save the clusters, frees them at the end.

    vec is the head of the first datapoint.
    centroids is the first centroid from the previous iteration.
    d is the length of the vectors.
    k is the number of centroids.
    */

    Cluster *first_cluster = malloc(sizeof(Cluster));
    Cluster *cluster = first_cluster;

    DPE *first_new_centroid = NULL;
    DPE *new_centroid = NULL;

    int i;

    for (i = 0; i < k; i++) {
        /*
        creates k empty clusters.
        */

        cluster->next_cluster = malloc(sizeof(Cluster));
        cluster = cluster->next_cluster;
    }

    while (vec != NULL) {
        /*
        appends vec to its cluster.
        */

        add_vec_to_closest_cluster(vec, centroids, first_cluster, d, k);
        vec = vec->next_dp;
    }

    cluster = first_cluster;

    /*
    first updates the first centroid.
    */

    first_new_centroid = update_centroid(cluster->first_vector_head, d, NULL);
    new_centroid = first_new_centroid;
    cluster = cluster->next_cluster;

    /*
    and only after setting first_new_centroid, updates the rest.
    */

    for (i = 1; i < k; i++) {
        new_centroid = update_centroid(cluster->first_vector_head, d, new_centroid);
        cluster = cluster->next_cluster;
    }

    /*free_clusters(first_cluster);*/

    return first_new_centroid;
}


int are_all_centroid_deltas_less_than_epsilson(DPE *prev_centroid, DPE *new_centroid, int d, int k) {
    int i;

    for (i = 0; i < k; i++) {
        if (distance(prev_centroid, new_centroid, d) >= EPS)
            return 0;
        
        prev_centroid = prev_centroid->next_dp;
        new_centroid = new_centroid->next_dp;
    }

    return 1;
}


DPE* k_means(DPE *datapoints, int d, int k, int iter) {
    /*
    the actual k-means algorithm
    makes repeated iterations until convergence and updates the centroids
    */

    int i;

    DPE *centroids, *new_centroids;
    DPE *centroid_vec, *vec = datapoints;

    /*
    sets the first iteration to be the first k vectors.
    */

    centroid_vec = create_vector(d, NULL, vec);
    centroids = centroid_vec;
    new_centroids = centroids;

    for (i = 1; i < k; i++) {
        vec = vec->next_dp;
        centroid_vec = create_vector(d, centroid_vec, vec);
    }

    /*
    makes repeated iterations until convergence and updates the centroids.
    */
    
    for (i = 1; i < iter; i++) {
        centroids = new_centroids;
        new_centroids = iteration(datapoints, centroids, d, k);

        if (are_all_centroid_deltas_less_than_epsilson(centroids, new_centroids, d, k) == 1) {
            free_dpe_matrix(centroids);
            break;
        }
        
        /*free(centroids);*/
        /*free_dpe_matrix(centroids);*/
    }

    return new_centroids;
}


int get_line_length(char *line) {
    int i = 0, len = 1;

    while (line[i]) {
        if (line[i] == ',')
            len++;
        
        i++;
    }

    return len;
}


DPE* get_vector_from_line(char *line, int d, DPE *prev_vec) {
    int i;

    DPE *head = create_vector(d, prev_vec, NULL), *vec;
    vec = head;

    for (i = 0; i < d; i++) {
        vec->value = strtod(line, &line);
        line++;
        vec = vec->next_entry;
    }

    return head;
}


int main(int argc, char *argv[]) {
    long k, iter;
    char *endptr, buffer[1024];
    int d, n = 1, i;
    DPE *first_vector, *vec, *centroid_head, *centroid_dpe;

    if (argc != 2 && argc != 3) {
        printf("An Error Has Occurred!\n");
        exit(1);
    }

    k = strtod(argv[1], &endptr);

    if (*endptr != '\0') {
        printf("Incorrect number of clusters!\n");
        exit(1);
    }

    if (argc == 3) {
        iter = strtol(argv[2], &endptr, 10);

        if (*endptr != '\0') {
            printf("Incorrect maximum iteration!\n");
            exit(1);
        }
    } else {
        iter = 400;
    }

    /*
    reading the file.
    */

    /*
    get first line and d.
    */

    fgets(buffer, sizeof(buffer), stdin);

    d = get_line_length(buffer);
    first_vector = get_vector_from_line(buffer, d, NULL);
    vec = first_vector;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        vec = get_vector_from_line(buffer, d, vec);
        n++;
    }

    /*
            FOR DEBUG:

            printf("\nd=%d, k=%lu iter=%lu n=%d\n", d, k, iter, n);

            while (vec) {
                printf("%f,", vec->value);
                vec = vec->next_entry;
            }
    */

    /*
    validate k
    */

    if (k <= 1 || k >= n) {
        printf("Incorrect number of clusters!\n");
        exit(1);
    }

    /*
    validate iter
    */

    if (k <= 1 || k >= 800) {
        printf("Incorrect maximum iteration!\n");
        exit(1);
    }
    
    centroid_head = k_means(first_vector, d, k, iter);

    for (i = 0; i < k; i++) {
        centroid_dpe = centroid_head;

        while (centroid_dpe != NULL) {
            if (centroid_dpe != centroid_head)
                printf(",");
            
            printf("%.4f", centroid_dpe->value);

            centroid_dpe = centroid_dpe->next_entry;
        }

        printf("\n");

        centroid_head = centroid_head->next_dp;
    }
    



    return 0;
}


/* compile with
gcc -ansi -Wall -Wextra -Werror -pedantic-errors hw1.c -lm -o kmeans
*/
