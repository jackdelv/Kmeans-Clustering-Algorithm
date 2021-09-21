# Kmeans-Clustering-Algorithm
Implementation of the Kmeans Clustering Algorithm using C++ and std::valarray

temp.img is the result of the clustering algorithm on the iris dataset.
These two lines are what produced that image.
$ ./kmeans iris.tsv 4 4 > temp.tsv\n
$ gnuplot -e 'set terminal png; set output "temp.png"; plot "temp.tsv" using 3:4:1:2 with points pt var lc var;'\n
![image](https://user-images.githubusercontent.com/85182563/134194453-5533a0e7-cd6b-4dca-8bee-345380e80792.png)
