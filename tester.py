from subprocess import call

call(["gcc", "-O3", "-fopenmp",  "-msse4", "alternate.c"])

call(["echo", "Running ./a.out 32 32 1 16 16 20..."])
call(["./a.out", "32", "32", "1", "16", "16", "20"])

call(["echo", "Running ./a.out 100 100 3 32 128 20..."])
call(["./a.out", "100", "100", "3", "32", "128", "20"])

call(["echo", "Running ./a.out 100 100 3 256 512 20..."])
call(["./a.out", "100", "100", "3", "256", "512", "20"])

call(["echo", "Running ./a.out 100 100 5 256 256 200..."])
call(["./a.out", "100", "100", "5", "256", "256", "200"])

#call(["echo", "Running ./a.out 300 300 7 512 512 500..."])
#call(["./a.out", "300", "300", "7", "512", "512", "500"])

call(["echo", "Running ./a.out 20 20 3 1024 1024 20..."])
call(["./a.out", "20", "20", "3", "1024", "1024", "20"])

call(["echo", "Running ./a.out 1000 1000 3 32 32 20..."])
call(["./a.out", "1000", "1000", "3", "32", "32", "20"])
