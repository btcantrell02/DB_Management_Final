# DB_Management_Final

# Simplified SELECT Statement Implementation for MLS Relation
This project involves implementing a simplified SELECT statement to retrieve information from a mulit-level security (MLS) relation. Each row in the MLS relation has an additional attribute named "TC" (classification), which can have one of four values: 4 (Top Secret), 3 (Secret), 2 (Confidential), and 1 (Unclassified).

## Installation
### Prerequisites
- C++ compiler (e.g. `g++`)

### Steps
1. Clone the repository

`git clone https://github.com/btcantrell02/DB_Management_Final.git`

`cd DB_Management_Final`

2. Compile the program:

`make clean`

`make`

### Usage
1. Run the program:

`./a.out <security_clearance_level>`

Replace `<security_clearance_level>` with an integer (1-4) to indicate the user's security clearance.

2. Input the SELECT statement:

The program will prompt you to enter a SELECT statement. The syntax of the SELECT is shown below. It consists of SELECT, FROM, WHERE, AND ORDERBY clauses, and ends with a semicolon.

`SELECT <columns> FROM <table> WHERE <conditions> ORDERBY <sorting criteria>`

The SELECT and FROM clauses are required. The FROM clause will only take one table. The columns of the SELECT clause could be * (to include all the columns including TC in the table) or a list to either include or exclude one or more columns. To include a column, use &lt;column&gt;:1, and to exclude a column, use &lt;column&gt;:0. The list cannot have both inclusion and exclusion specifications. If the list contains mulitple columns, they are seperated by commas.

The WHERE and ORDERBY clauses are optional. The conditions of the WHERE clause can have one or more conditions. If there are mulitple conditions, commas will be used to seperate them. Each condition is always of &lt;column&gt;&lt;op&gt;&lt;ref_value&gt; format. &lt;op&gt; is one of the six relational operators (=, &lt;&gt;, &gt;=, &lt;=, &gt;, and &lt;). The left side is always a column name, and the right side is a reference value that could be a string or a number, depending on the data type of the column. The sorting criteria of the ORDERBY clause could be one criterion or mulitple criteria seperated by commas. Each criterion consists of a column name followed by :1 for ascending order or :-1 for descending order. The column to be used for sorting must be an output column.

The following are some examples of the SELECT statement:
- SELECT * FROM EMPLOYEE;
- SELECT LNAME:1,FNAME:1,SALARY:1 FROM EMPLOYEE;
- SELECT TC:0 FROM EMPLOYEE;
- SELECT SALARY:0, TC:0 FROM EMPLOYEE;
- SELECT * FROM EMPLOYEE WHERE TC>=3;
- SELECT * FROM EMPLOYEE WHERE SALARY>=30000;
- SELECT * FROM EMPLOYEE WHERE TC<3, SALARY>=30000;
- SELECT * FROM EMPLOYEE ORDERBY SALARY:-1;
- SELECT * FROM EMPLOYEE ORDERBY SALARY:-1, LNAME:1;
- SELECT LNAME:1,FNAME:1,SALARY:1 FROM EMPLOYEE WHERE SALARY >= 30000 ORDERBY SALARY:1;
- SELECT * FROM EMPLOYEE ORDERBY TC:-1, SALARY:-1, LNAME:1;
- SELECT * FROM EMPLOYEE WHERE LNAME >= K ORDERBY LNAME:1;
- SELECT * FROM EMPLOYEE ORDERBY SEX:1, SALARY:-1, LNAME:1;
- SELECT TC:0 FROM EMPLOYEE ORDERBY SUPERSSN:1, SSN:1;

3. Output: 
The program will display the result of the SELECT statement in CSV format, including column names as the first row.

### Example
`./a.out 4`

After running the command, you can enter a select statement such as:

`SELECT * FROM EMPLOYEE WHERE TC<3 ORDERBY SALARY:-1;`

To exit the program, type: 

`EXIT`

## Contact
For any questions, please contact btcantrell02@gmail.com