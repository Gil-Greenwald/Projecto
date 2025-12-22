import random
import math

from hw1 import *


def test_distance():
    if distance((3.0,), (4.0,)) != 1.0:
        print(1.1)
    
    if distance((0.0, 2.0), (3.0, -2.0)) != 5.0:
        print(1.2)
    
    if distance((0.0,) * 10, tuple(float(i) for i in range(5))) != math.sqrt(30):
        print(1.3)


def test_closest_cluster_index():
    dp = (0.0,) * 3
    centroids = (
        (0.0, 0.0, 0.0),
        (1.0, 0.0, 0.0),
        (0.0, 1.0, 0.0)
    )

    if closest_cluster_index(dp, centroids) != 0:
        print(2.1)
    
    dp = (0.0,) * 3
    centroids = (
        (0.0, 0.25, 0.5),
        (1.0, 0.0, 0.0),
        (0.0, 1.0, 0.0)
    )

    if closest_cluster_index(dp, centroids) != 0:
        print(2.2)


def test_update_centroid():
    cluster = [
        (1.0, 0.0, 0.0),
        (0.0, 1.0, 0.0),
        (0.0, 0.0, 1.0)
    ]

    if update_centroid(cluster) != (1/3,) * 3:
        print(3.1)
    
    cluster = [
        (0.0, 0.0, 0.0, 1.0),
        (1.0, 0.0, 0.0, 1.0),
        (0.0, 1.0, 0.0, 1.0),
        (0.0, 0.0, 1.0, 1.0),
        (1.0, 1.0, 2.0, 1.0)
    ]

    if update_centroid(cluster) != (0.4, 0.4, 0.6, 1.0):
        print(3.2)


def test_iteration():
    centroids = (
        (0.0, 0.0, 0.0),
        (5.0, 0.0, 5.0),
        (0.0, 5.0, 0.0)
    )

    clustered_datapoints = [
        [
            (0.0, 0.0, 0.0),
            (1.0, 0.0, 0.0),
            (1.0, 1.0, 0.0),
            (0.0, 0.0, 2.0)
        ],
        [
            (4.0, 0.0, 5.0),
            (5.0, 1.0, 6.0),
            (5.0, 0.0, 4.0)
        ],
        [
            (1.0, 4.0, 0.0),
            (0.0, 6.0, 0.0)
        ]
    ]

    datapoints = [dp for cluster in clustered_datapoints for dp in cluster]
    random.shuffle(datapoints)

    result = {
        (0.5, 0.25, 0.5),
        (14/3, 1/3, 5.0),
        (0.5, 5.0, 0.0)
    }

    if set(iteration(tuple(datapoints), centroids)) != result:
        print(4.1)
    
    centroids = (
        (0.0, 0.0, 0.0, 0.0),
        (5.0, 5.0, 5.0, 5.0),
        (10.0, 10.0, 10.0),
        (-10.0, 0.0, 0.0, 0.0)
    )

    clustered_datapoints = [
        [
            (0.0, 0.0, 0.0, 0.0),
            (1.0, 0.0, 1.0, 0.0),
            (2.0, 1.0, 0.0, 3.0),
            (1.0, 4.0, 3.0, 1.0)
        ],
        [
            (4.0, 5.0, 6.0, 7.0),
            (5.0, 4.0, 6.0, 5.0),
            (4.5, 6.0, 3.0, 5.0)
        ],
        [
            (9.0, 8.0, 7.0, 6.0),
            (10.0, 11.0, 9.0, 11.0),
            (8.0, 8.0, 8.0, 8.0),
            (12.0, 11.0, 150.0, 0.0),
            (5.0, 10.0, 10.0, 9.0)
        ],
        [
            (-9.0, 0.0, 0.0, 1.0)
        ]
    ]

    datapoints = [dp for cluster in clustered_datapoints for dp in cluster]
    random.shuffle(datapoints)

    result = {
        (1.0, 1.25, 1.0, 1.0),
        (4.5, 5.0, 5.0, 17/3),
        (8.8, 9.6, 36.8, 6.8),
        (-9.0, 0.0, 0.0, 1.0)
    }

    if set(iteration(tuple(datapoints), centroids)) != result:
        print(4.2)


def test_iteration_delta_smaller():
    prev_centroids = (
        (0.0, 0.0, 0.0),
        (5.0, 0.0, 5.0),
        (0.0, 5.0, 0.0)
    )
    new_centroids = (
        (0.5, 0.25, 0.5),
        (14/3, 1/3, 5.0),
        (0.5, 5.0, 0.0)
    )

    if iteration_delta_smaller(prev_centroids, new_centroids, 0.001) is True:
        print(5.1)
    
    prev_centroids = (
        (0.0, 0.0, 0.0, 0.0),
        (5.0, 5.0, 5.0, 5.0),
        (10.0, 10.0, 10.0),
        (-10.0, 0.0, 0.0, 0.0)
    )
    new_centroids = (
        (0.0, 0.0, 0.0, 0.0),
        (4.5, 5.0, 5.0, 17/3),
        (8.8, 9.6, 36.8, 6.8),
        (-9.0, 0.0, 0.0, 1.0)
    )

    if iteration_delta_smaller(prev_centroids, new_centroids, 0.001) is True:
        print(5.2)


def test_k_means():
    pass


test_distance()
test_closest_cluster_index()
test_update_centroid()
test_iteration()
test_iteration_delta_smaller()
test_k_means()
