Sets
i index of motors /1*8/
;
Alias (i, j);
Parameters
a(i) requiered number of motors/
1 20
2 10
3 50
4 20
5 80
6 40
7 20
8 50/
u(i) coefficients/
1 1
2 0.9
3 0.8
4 0.7
5 0.6
6 0.5
7 0.4
8 0.3
/
sto /150/
Variables
number(i);
Parameter c(i) cost of production;
c(i) = 30 * u(i);
Parameter c0(i) cost of openning the plant;
c0(i) = 600 + 500 * u(i);

Binary Variable
isOpen(i)
;

Free Variable z;

Equations
target
*constr_number
constr_number1
constr_number2
constr_number3
constr_number4
constr_number5
constr_number6
constr_number7
constr_number8
constr_isOpen
extra
;

target.. z =e= sum(i, c0(i) * isOpen(i) + number(i) * c(i));
*constr_number(j).. sum(i$(i.val <= j.val), number(i)) =g= sum(i$(i.val <= j.val), a(i));
constr_number1.. sum(i$(i.val <= 1), number(i)) + sto =g= sum(i$(i.val <= 1), a(i));
constr_number2.. sum(i$(i.val <= 2), number(i)) + sto =g= sum(i$(i.val <= 2), a(i));
constr_number3.. sum(i$(i.val <= 3), number(i)) + sto =g= sum(i$(i.val <= 3), a(i));
constr_number4.. sum(i$(i.val <= 4), number(i)) + sto =g= sum(i$(i.val <= 4), a(i));
constr_number5.. sum(i$(i.val <= 5), number(i)) + sto =g= sum(i$(i.val <= 5), a(i));
constr_number6.. sum(i$(i.val = 6), number(i)) =g= sum(i$(i.val = 6), a(i));
constr_number7.. sum(i$(i.val <= 7), number(i)) + sto =g= sum(i$(i.val <= 7), a(i));
constr_number8.. sum(i$(i.val <= 8), number(i)) + sto =g= sum(i$(i.val <= 8), a(i));
constr_isOpen(j).. isOpen(j) * sum(i, a(i)) =g= number(j);
extra(j).. number(j) =g= 0;

Model Motor /all/;
Option optcr = 0.0;
Solve Motor using mip minimazing z;
Display z.l, target.l, number.l;
