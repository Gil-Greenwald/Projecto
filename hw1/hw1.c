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



struct DPE {
    double value;
    struct DPE *next_entry;
    struct DPE *next_dp;
};
typedef struct DPE DPE;


double distance(DPE *vec1, DPE *vec2) {
    /*
    calculates the distance of vec1 from vec2 based on the Pythagorean theorem (see pdf for definition)
    vec1 and vec2 are the first coordinates of datapoints, like A_i1, A_j1 in the matrix.
    */

    double sum = 0.0, diff;

    while (vec1 != NULL) {
        /*
        iterate over all the entries of the two vectors, until vec1 is exhausted
        */

        diff = vec1->value - vec2->value;
        sum += diff * diff;

        vec1 = vec1->next_entry;
        vec2 = vec2->next_entry;
    }

    return sqrt(sum);
}


int closest_cluster_index(DPE *vec, DPE *centroids) {
    /*
    based on a datapoint vector and K centroids, returns the index of the closest centroid to the datapoint.
    e.g., if dp is closest to centroids[1], returns 1.
    vec is the first coordinate of a datapoint, like A_i1 in the matrix.
    */

    int curr_min = 0, i = 0;

    while (centroids != NULL) {
        /*
        iterate over all vectors in centroids.
        for each one, calculate its distance from vec, and save the minimal index.
        */

        if (distance(vec, centroids) < curr_min)
            curr_min = i;

        i++;
        centroids = centroids->next_dp;
    }

    return curr_min;
}


DPE* update_centroid(DPE *cluster) {
    /*
    # based on a (linked) list of datapoints, calculates their centroid using the mean vector (see pdf for definition).
    cluster is the first coordinate of the first datapoint of the cluster, like A_11.
    not all the datapoints from the file are proccessed here, only a certain cluster.
    */

    double sum = 0.0;
    int i = 0;

    DPE *cluster_dpe = cluster;

    DPE *new_centroid = malloc(sizeof(DPE));
    DPE *new_centroid_dpe = new_centroid;

    while (cluster != NULL) {
        /*
        iterate over the coordiantes of the clusters and then iterate over the datapoinsts.
        the idea is to calculate each coordiante of the resulting vector by
        going over each the same coordinate of every datapoint.
        */

        cluster_dpe = cluster;
        sum = 0.0;
        i = 0;

        while (cluster_dpe != NULL) {
            /*
            iterate over all datapoints in the cluster, for the same coordinate,
            and save the mean into the value of the next coordiante in the new centroid.
            */

            sum += cluster_dpe->value;
            i++;
            cluster_dpe = cluster_dpe->next_dp;
        }

        new_centroid_dpe->value = sum / i;

        cluster = cluster->next_entry;

        new_centroid_dpe->next_entry = malloc(sizeof(DPE));
        new_centroid_dpe = new_centroid_dpe->next_entry;

        /*
        note that we create an extra DPE in the last entry.
        because of that, when passing centroids to distance, centroids must be the second argument.
        also, this is taken care of later (in code which I haven't already written).
        */
    }

    return new_centroid;
}


DPE* iteration(DPE *datapoints, DPE *centroids) {
    
}


int main() {
    return 0;
}


/* compile with
gcc -ansi -Wall -Wextra -Werror -pedantic-errors hw1.c -lm -o hw1
*/