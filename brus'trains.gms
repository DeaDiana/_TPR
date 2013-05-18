Sets
i index for type of train /1*5/
j index for schema /1*9/
k index for type of detail /1*3/
;
Parameters
tr(i) i type of train has i vagons/
1 1
2 2
3 3
4 4
5 5/
resource /100/
;
Table
t(j,k) j-schema has t(j_k) of k-type details
  1 2 3
1 2 0 0
2 1 2 0
3 0 4 0
4 1 1 1
5 1 0 3
6 0 2 3
7 0 3 1
8 0 1 5
9 0 0 9
;
Integer Variables
x(i) number of i-type trains
q(j) number of j-type schemas
;
Free Variable
wagon_number target function
;
Equations
func main func
res_constr
Aconstr
Bconstr
Cconstr
;

func.. wagon_number =e= sum(i, x(i)*tr(i));
res_constr.. sum(j, q(j)) =l= resource;
Aconstr.. sum(i, x(i)*(tr(i)+1)) =l= sum(j, q(j)*t(j,'1'));
Bconstr.. sum(i, x(i)) =l= sum(j, q(j)*t(j,'2'));
Cconstr.. sum(i, x(i)*2*(tr(i)+1)) =l= sum(j, q(j)*t(j,'3'));
Model Brus /all/;
Option optcr = 0.0;
Solve Brus using mip maximazing wagon_number;
Display x.l, wagon_number.l, res_constr.l;
