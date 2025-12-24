#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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
        return NULL;
    
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
            return NULL;
        
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


void add_vec_to_closest_cluster(DPE *vec, DPE *centroids, Cluster *clusters, int d) {
    /*
    finds the closest centroid to vec, and appends it to the corresponding cluster.
    vec is the head of a vector,
    centroids is the head of the first centroid,
    clusters is a pointer to the first cluster,
    and d is the length of all the vectors.
    */

    int min_index = 0, i = 0;

    while (centroids != NULL) {
        /*
        iterates over all vectors in centroids.
        for each one, calculates its distance from vec, and saves the minimal index.
        */

        if (distance(vec, centroids, d) < min_index)
            min_index = i;

        i++;
        centroids = centroids->next_dp;
    }

    for (i = 0; i < min_index; i++) {
        /*
        finds the correct cluster based on min_index
        */
        
        clusters = clusters->next_cluster;
    }

    create_vector(d, clusters->last_vector_head, vec);
}


DPE* update_centroid(DPE *cluster, int d) {
    /*
    # based on a (linked) list of datapoints, calculates their centroid using the mean vector (see pdf for definition).
    cluster is the first coordinate of the first datapoint of the cluster, like A_11.
    d is the length of the vectors.
    not all the datapoints from the file are proccessed here, only a certain cluster.
    */

    double sum = 0.0;
    int i, num_of_vectors = 0;

    DPE *cluster_dpe = cluster;

    DPE *new_centroid_head = create_vector(d, NULL, NULL);
    DPE *new_centroid_dpe = new_centroid_head;

    for (i = 0; i < d; i++) {
        /*
        iterates over the coordiantes of the clusters and then iterate over the datapoinsts.
        the idea is to calculate each coordiante of the resulting vector by
        going over each the same coordinate of every datapoint.
        */

        cluster_dpe = cluster;
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

        cluster = cluster->next_entry;
        new_centroid_dpe = new_centroid_dpe->next_entry;
    }

    return new_centroid_head;
}


DPE* iteration(DPE *datapoints, DPE *centroids) {
    /*
    for each centroid, stores a list of its closest datapoints.
    assume datapoints[i] is closest to centroids[j], then dp_clusters[j] contains datapoints[i].
    then calculates the centroids using update_centroid and returns all the updated centroids.
    */

    /*
    IDEA:
    save a linked list of length k which will have, for each cluster, the last seen vector in this cluster.
    go over all the vectors in the datapoints, and by getting the correct cluster index, go iteratively to the correct cluster and append the new vector
    */

    return datapoints;
}


int main() {
    return 0;
}


/* compile with
gcc -ansi -Wall -Wextra -Werror -pedantic-errors hw1.c -lm -o hw1
*/
