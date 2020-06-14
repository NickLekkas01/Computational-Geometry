from math import sqrt
from random import randint, uniform


from scipy.spatial import Voronoi, voronoi_plot_2d

from sklearn.cluster import KMeans

from datasets import *

import matplotlib.pyplot as plt

def euclidean_distance(p1, p2):
    if len(p1) != len(p2):
        print('Cannot find distance')
        return -1

    sum = 0

    for i in range(0,len(p1)):
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i])
    
    return sqrt(sum)

def K_means(data, centers):
    centers_num = len(centers)

    prev_labels = []
    labels = []

    while 1:
        for point in data:
            min_center_index = -1
            min_dist = -1

            for i in range(0, centers_num):
                center = centers[i]
                curr_dist = euclidean_distance(point, center)

                if (min_center_index == -1) or (curr_dist < min_dist):
                    min_center_index = i
                    min_dist = curr_dist
            
            labels.append(min_center_index)

        if labels == prev_labels:
            break
        
        new_centers = [[0]*len(data[0]) for c in centers]
        labels_occurences = [0 for c in centers]

        for point, label in zip(data, labels):
            labels_occurences[label] += 1

            for i in range(0, len(new_centers[label])):
                new_centers[label][i] += point[i]
            
        for i in range(0, len(labels_occurences)):
            for j in range(0, len(new_centers[i])):
                if labels_occurences[i] != 0:
                    new_centers[i][j] = new_centers[i][j] / labels_occurences[i]

        centers = new_centers
        prev_labels = labels
        labels = []

    return labels

def voronoi_center_init(data, clusters_num):
    vertices = Voronoi(data).vertices
    voronoi_plot_2d(Voronoi(data))
    plt.show()
    vertices = [tuple(v) for v in vertices]
    
    # Finding their circle radius and their circumference points
    vertice_info = {}

    for v in vertices:
        min_radius = -1
        min_circumference_points = []

        for p in data:
            curr_dist = round(euclidean_distance(v,p),10) # I do this because problems with number precision will arise

            if (min_radius == -1) or (curr_dist < min_radius):
                min_radius = curr_dist
                min_circumference_points = [tuple(p)]
            elif curr_dist == min_radius:
                min_circumference_points.append(tuple(p))
            
        vertice_info[tuple(v)] = tuple([min_radius, tuple(min_circumference_points)])

        assert len(min_circumference_points) >= 3

    vertices = sorted(vertices,key=lambda v: vertice_info[v][0], reverse=True)

    for i in range(0,len(vertices) - 1):
        assert vertice_info[vertices[i]][0] >= vertice_info[vertices[i+1]][0]
    
    ccenter = set()
    Test = set()
    Temp = set()

    for i in range(0, len(vertices)):
        curr_vertice = vertices[i]

        vertice_radius = vertice_info[curr_vertice][0]
        vertice_circumpoints = set(vertice_info[curr_vertice][1])

        Test = Test.union(vertice_circumpoints)

        test_list = list(Test)

        for i in range(0,len(test_list)-1):
            for j in range(i+1,len(test_list)):
                if euclidean_distance(test_list[i], test_list[j]) < vertice_radius:
                    if test_list[j] in Test:
                        Test.remove(test_list[j])
        
        if len(ccenter) == 0:
            ccenter = set(Test)

            if len(ccenter) >= clusters_num:
                break

            Test = set()
            continue
        
        for center_point in ccenter:
            for test_point in Test:
                curr_dist = euclidean_distance(center_point, test_point)

                if curr_dist < vertice_radius:
                    Temp.add(test_point)
        
        ccenter = ccenter.union(Test.difference(Temp))

        if len(ccenter) >= clusters_num:
            break
        
        Test = set()
        Temp = set()

    return list(ccenter)[0:clusters_num]  # In case the algorithm does not terminate on if(check if you do something wrong)

def Voronoi_K_Means(data, clusters_num):
    init_centers = voronoi_center_init(data, clusters_num)

    plt.scatter([p[0] for p in data],[p[1] for p in data], c='red')
    plt.scatter([p[0] for p in init_centers], [p[1] for p in init_centers], c='black')
    plt.title('Centers of algorithm')
    plt.show()

    return K_means(data, init_centers)

def plot_cluster_results(data, labels, title=""):
    dif_labels = set(labels)
    print('Found', len(dif_labels), 'clusters')
    colors = ['black','grey', 'lightcoral', 'firebrick', 'red', 'orangered',
    'chocolate', 'orange', 'darkgoldenrod', 'yellow', 'darkolivegreen', 'lawngreen',
    'turquoise','teal', 'navy', 'indigo', 'magenta']

    for label in dif_labels:
        points_x = [p[0] for p, l in zip(data, labels) if l == label]
        points_y = [p[1] for p, l in zip(data, labels) if l == label]

        plt.scatter(points_x, points_y, c=colors[label])
    
    plt.title(title)
    plt.show()

#points = [[1,1],[2,1],[2,2],[10,1],[10,2],[11,1],[1,10],[2,10],[2,11],[10,10],[11,10],[11,11],[5,5],[6,5],[6,6]]

points = r15()
plt.scatter([p[0] for p in points], [p[1] for p in points])
plt.show()

# If I do 8 means the proposed algorithm performs better than
# the Kmeans++. If I do 15 means, the opposite(check if you did something wrong)

# If I give completely wrong number of clusters the proposed algorithm
# performs really bad
num_clusts = 15
labels = Voronoi_K_Means(points, num_clusts)
plot_cluster_results(points, labels, 'Voronoi K-Means')

kmeans = KMeans(num_clusts,n_init=1).fit(points)
plot_cluster_results(points, kmeans.labels_, 'K-Means++')
