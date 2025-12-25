import math
from sys import argv, stdin


vec = tuple[float, ...]


def distance(vec1: vec, vec2: vec) -> float:
    # calculates the distance of vec1 from vec2 based on the Pythagorean theorem (see pdf for definition)

    return math.sqrt(
        sum(
            (v1 - v2) ** 2
            for v1, v2 in zip(vec1, vec2)
        )
    )


def closest_cluster_index(dp: vec, centroids: tuple[vec, ...]) -> int:
    # based on a datapoint vector and K centroids, returns the index of the closest centroid to the datapoint
    # e.g., if dp is closest to centroids[1], returns 1

    return min(
        range(len(centroids)),
        key=lambda i: distance(dp, centroids[i])
    )


def update_centroid(cluster: list[vec]) -> vec:
    # based on a list of datapoints, calculates their centroid using the mean vector (see pdf for definition)

    d = len(cluster[0])

    return tuple(
        sum(
            vector[i]
            for vector in cluster
        ) / len(cluster)
        for i in range(d)
    )


def iteration(datapoints: tuple[vec, ...], centroids: tuple[vec, ...]) -> tuple[vec, ...]:
    # for each centroid, stores a list of its closest datapoints
    # assume datapoints[i] is closest to centroids[j], then clusters[j] contains datapoints[i]
    # then calculates the centroids using update_centroid and returns all the updated centroids

    clusters: list[list[vec]] = [[] for _ in range(len(centroids))]

    for vector in datapoints:
        cluster_index = closest_cluster_index(vector, centroids)
        clusters[cluster_index].append(vector)
    
    return tuple(
        update_centroid(clusters[j])
        for j in range(len(clusters))
    )


def k_means(datapoints: tuple[vec, ...], k: int, iter: int = 400, eps: float = 0.001) -> tuple[vec, ...]:
    # the actual k-means algorithm
    # makes repeated iterations until convergence and updates the centroids

    centroids: tuple[vec, ...] = datapoints[:k]
    new_centroids = centroids

    for _ in range(iter):
        centroids = new_centroids
        new_centroids = iteration(datapoints, centroids)

        if all(
            distance(prev_cent, new_cent) < eps
            for prev_cent, new_cent in zip(centroids, new_centroids)
        ):
            break
    
    return new_centroids


def main():
    # checks whether the input is valid:
    # input should contain the file name, K, iter, the stupid "<" and the input file
    # K should be an int, and satisfy 1 < K < N == number of rows in the input file
    # iter should be an int, and satisfy 1 < iter < 800

    if len(argv) not in (2, 3):
        print("An Error Has Occurred!")
        exit()
    
    if not argv[1].isdigit():  # K
        print("Incorrect number of clusters!")
        exit()
    
    k = int(argv[1])
    
    if len(argv) == 3 and not argv[2].isdigit():  # iter
        print("Incorrect maximum iteration!")
        exit()
    
    iter = int(argv[2]) if len(argv) == 3 else 400
    
    try:
        datapoints = tuple(
            tuple(
                float(dp)
                for dp in row.split(",")
            )
            for row in stdin.readlines()
        )
    except Exception:
        print("An Error Has Occurred!")
        exit()
    
    if not (1 < k < len(datapoints)):
        print("Incorrect number of clusters!")
        exit()
    
    if not (1 < iter < 800):
        print("Incorrect maximum iteration!")
        exit()

    for cluster in k_means(datapoints, k, iter):
        print(",".join(f"{c:.4f}" for c in cluster))
    
    print()

    # when returning the file, truncate floats to 4 decimal places


main()
