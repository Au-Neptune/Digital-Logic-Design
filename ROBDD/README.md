* step 1:

	Make sure your Linux OS has g++.
	You can use `sudo yum install gcc gcc-c++` to install.
* step 2:

	Download the file in your computer(OS:Linux).
	Type `g++ -o a ROBDD.cpp` in your CMD.
	#Remember you need to use cd change your directory where ROBDD.cpp is.
* step 3:

	Type `./a input.pla output.dot` to execute the program.
	If it works, you will see the result on CMD, and there is a file named output.dot which includes the ROBDD format in the folder.
* step 4 (optional):

	Type `dot -T png output.dot > output.png` to get the PNG file.
