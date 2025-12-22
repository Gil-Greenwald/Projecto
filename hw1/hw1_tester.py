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
        (0.0, 0.0, 1.0, 1.0)
    ]

    if update_centroid(cluster) != (0.25, 0.25, 0.25, 1.0):
        print(3.2)


def test_iteration():
    pass


def test_iteration_delta_smaller():
    pass


def test_k_means():
    pass


test_distance()
test_closest_cluster_index()
test_update_centroid()
test_iteration()
test_iteration_delta_smaller()
test_k_means()
