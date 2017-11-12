Version 1.1
 	1.the difference between v1.0 and v1.1 is the Counter of program ,that the handle method is different. 20170905

   20170905
/------------------------/

2017-11-9日差分直流软件更改：
?	在进行自学习SET2时，软件会将信号的最大值SET2-MAX保留在EEPROM中，阀值还是SET1的值加上此最大值，除以2，得到最终阀值。
?	在正常生产时，每50US对信号值进行四次采样，最后得到信号的AD值，记为S-AD值。若S-AD值大于等于阀值，同时又小于等于SET2-MAX+50时，A=1. 若S-AD值小于等于阀值-DEL值，A=0. 50与70这两个数据也包含有应差概念。
?	当A=1后，并不马上与REGISTER B进行同或运算然后输出到OUT1上，而是还要看后续的16个S-AD值的数据变化，若后续的16个数据都小于等于SET2-MAX+50，则软件将先前的REGISTER A值与B值同或运算后，输出到OUT1上。若后续的16个数据只要有一个或多个数据大于等于SET2-MAX+70，则先前的A值作废，软件不将AB同或值输出到OUT1上，OUT1还是保持当前的输出状态.实际上是软件测得A值后，在等16个数据后，在决定是否将AB同或值输出到OUT1
OUT2的功能不变
