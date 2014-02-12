TablePrinter
==================

A header-only C++ library for better table display in console.

#Features supported:
1.Line number
2.Column header (Column width)
3.Table header
4.Support output to file.

Ex:
<pre>
+-----------------------------------+  
|            Test Table             |  
+---+----+--------------------------+  
|   |col1|Col2's header is very long|  
+---+----+--------------------------+  
|  0|   3|                       4.6|  
+---+----+--------------------------+  
|  1|  hi|                         6|  
+-----------------------------------+ 
</pre>

#Features to implement:
1.Fusioned rows and columns  
2.Color support  
Ex:
<pre>
+---------------------------------------+
|                Results                |
+---------------------------------------+
|             |        Contestant       |
+ Competition +-------------------------+
|             |  John | Andrea | Robert |
+-------------+-------+--------+--------+
| Swimming    |  1:30 |   2:05 |   1:15 |
+-------------+-------+--------+--------+
| Running     | 15:30 |  14:10 |  15:45 |
+-------------+-------+--------+--------+
</pre>