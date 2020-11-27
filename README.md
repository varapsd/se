<h3>Machine learning based thread Parallelism regulation in software transactional memory</h3>
<p>In this project we are parllellising the transaction in the dataset by the required number of threads. There are three modules in the system and this completly done in c language with openmp library.</p>
<p>In first step run the system with 10 datasets avaialble in the dataset folder. run command as </p>
<p> $ gcc progam1.c -fopenmp -o data</p>
<p> $ ./data </p>
<p> Now a new dataset is created for the multiple regression. Now run the multipule regression to get the equation to find the number of best threads for the transaction set</p>
<p> $ gcc main.c -Wall dist.c matrix.c -o train -lm </p>
<p> $ ./train </p>
<p> Now the system is ready for and the program.c file will execute the system with any transaction dataset by calculating the threads required from the multiple regression.</p>
<p> $ gcc program.c </p>
<p> $ ./a.out </p>
