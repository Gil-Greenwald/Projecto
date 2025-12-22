import math
from sys import argv


def distance(vec1: tuple[float, ...], vec2: tuple[float, ...]) -> float:
    return math.sqrt(
        sum(
            (v1 - v2) ** 2
            for v1, v2 in zip(vec1, vec2)
        )
    )


def closest_cluster_index(dp: tuple[float, ...], centroids: tuple[tuple[float, ...], ...]) -> int:
    return min(
        range(len(centroids)),
        key=lambda i: distance(dp, centroids[i])
    )


def update_centroid(cluster: list[tuple[float, ...]]) -> tuple[float, ...]:
    d = len(cluster[0])

    return tuple(
        sum(
            dp[i]
            for dp in cluster
        ) / d
        for i in range(d)
    )


def iteration(datapoints: tuple[tuple[float, ...], ...], centroids: tuple[tuple[float, ...], ...]) -> tuple[tuple[float, ...], ...]:
    # for each centroid, stores a list of closest datapoints.
    # assume datapoints[i] is closest to centroids[j], then dp_clusters[j] contains datapoints[i].

    dp_clusters: list[list[tuple[float, ...]]] = [[] for _ in range(len(centroids))]

    for dp in datapoints:
        cluster_index = closest_cluster_index(dp, centroids)
        dp_clusters[cluster_index].append(dp)
    
    return tuple(
        update_centroid(dp_clusters[j])
        for j in range(len(dp_clusters))
    )


def iteration_delta_smaller(prev_centroids: tuple[tuple[float, ...], ...], new_centroids: tuple[tuple[float, ...], ...], eps: float) -> bool:
    return all(
        distance(prev_cent, new_cent) < eps
        for prev_cent, new_cent in zip(prev_centroids, new_centroids)
    )


def k_means(datapoints: tuple[tuple[float, ...], ...], k: int, iter: int = 400, eps: float = 0.001) -> tuple[tuple[float, ...], ...]:
    centroids: tuple[tuple[float, ...], ...] = datapoints[:k]

    for _ in range(iter):
        new_centroids = iteration(datapoints, centroids)

        if all(
                distance(prev_cent, new_cent) < eps
                for prev_cent, new_cent in zip(centroids, new_centroids)
        ):
            break
    
    return centroids


def main():
    if len(argv) != 4:
        print("An Error Has Occurred!")
        exit()
    
    if not argv[1].isdigit():  # K
        print("An Error Has Occurred!")
        exit()
    
    k = int(argv[1])
    
    if not argv[2].isdigit():  # iter
        print("An Error Has Occurred!")
        exit()
    
    iter = int(argv[2])
    
    try:
        with open(argv[3], "r") as f:
            datapoints = tuple(
                tuple(
                    float(dp)
                    for dp in row.split(",")
                )
                for row in f.readlines()
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

    print(k_means(datapoints, k, iter))

    # when returning the file, truncate floats to 4 decimal places


if __name__ == "__main__":
    main()
