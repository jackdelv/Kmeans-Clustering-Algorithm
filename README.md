# Kmeans-Clustering-Algorithm
Implementation of the Kmeans Clustering Algorithm using C++ and std::valarray

KmeansHelper contains a few helper methods like distance and writeResults and declarations of namespaces which are useful.

delvecja.cpp is the main file that contains the main method and most of the useful functions. It takes three command line arguments and they are required. The first argument is always the file of tab seperated values to be clustered. The second argument is the number of columns to be read from the file. Each point can be 2 dimensional to n dimensional. The third argument is the number of clusters to be used in the algorithm. If the third argument is 0 then no clustering is done and the data points are just printed out.

temp.img is the result of the clustering algorithm on the iris dataset.

These two lines are what produced that image.

$ ./delvecja iris.tsv 4 4 > temp.tsv

$ gnuplot -e 'set terminal png; set output "temp.png"; plot "temp.tsv" using 3:4:1:2 with points pt var lc var;'

![image](https://user-images.githubusercontent.com/85182563/134194453-5533a0e7-cd6b-4dca-8bee-345380e80792.png)
