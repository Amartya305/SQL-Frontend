CREATE TABLE employee(
	emp_id INT ,
	emp_name VARCHAR ,
	basic INT,
	jn_dt DATE,
	PRIMARY KEY (emp_id)
	);

INSERT INTO employee(emp_id,emp_name,basic)
VALUES(1001,'JOHN',50000);

INSERT INTO employee VALUES (1002,'DAVE',30000);

INSERT INTO employee(emp_id,emp_name,basic)
VALUES(1003,'BOSE',20000);

INSERT INTO employee VALUES (1004,'FINN',70000);

INSERT INTO employee (emp_id,emp_name,basic)
VALUES(1005,'MARY',60000);

SELECT emp_name FROM employee
WHERE emp_id = 1003 ;

SELECT emp_id,emp_name,basic FROM employee 
WHERE basic BETWEEN 35000 AND 65000 ;

SELECT emp_name FROM employee
WHERE emp_id LIKE 's';

SELECT emp_id,emp_name,basic FROM employee
WHERE basic IN (20000,30000,70000) ;