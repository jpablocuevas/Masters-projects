import random 

random.seed ();

data = open ("rand_data.txt", "w");
nums = open ("rand_nums.txt", "w");

"""N = int (input ("N: "));
inf = int (input ("Lower bound: "));
sup = int (input ("Upper bound: ")); """

N = 10;
inf = -2.5;
sup = 2.5;

data.write (str(N) + "\t" + str (inf) + "\t" + str (sup));
data.close();

for i in range (0, N, 1):

	nums.write (str(random.uniform (sup, inf)) + "\n");

nums.close ();
