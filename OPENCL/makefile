

run_clean:
	g++ host/opencl.cpp -o ./executables/exe.out -std=c++11 -Wall -framework OpenCL
	./executables/exe.out


run: 
	g++ host/opencl.cpp -o ./executables/exe.out -std=c++11 -Wall -framework OpenCL
	rm ./IO/output.csv 
	./executables/exe.out >> ./IO/output.csv
	python2 ./visualization/data_post_processing.py

new_run:
	g++ new_host/opencl.cpp -o ./executables/new_exe.out -Wall -framework OpenCL
	./executables/new_exe.out 



rundb:
	g++ -g opencl.cpp -o ./executables/exe.out -Wall -framework OpenCL
	gdb ./exe.out



