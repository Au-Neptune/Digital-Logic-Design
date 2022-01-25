* step 1:
	
  Make sure your Linux OS has g++.
	You can use `sudo yum install gcc gcc-c++` to install.
* step 2:
	
  Download the file in your computer(OS:Linux).
	Type `g++ -o a source.cpp` in your CMD.
  
	Remember you need to use cd change your directory where mini.cpp is.
* step 3:

	Type `./a input.kiss output.kiss output.dot` to execute the program.
	If it works, you will see the files named output.kiss, output.dot and input.dot for original STG.
* step 4:

	Type `dot -T png output.dot > output.png` to get the PNG file.

---
![image](https://github.com/Au-Neptune/Digital-Logic-Design/blob/main/State%20Minimization/idea.png)
