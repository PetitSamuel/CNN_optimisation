from subprocess import call

call(["gcc", "-O3", "-fopenmp",  "-msse4", "alternate.c"])

call(["echo", "Running ./a.out 20 20 1 16 16 20..."])
call(["./a.out", "20", "20", "1", "16", "16", "20"])

call(["echo", "Running ./a.out 20 20 1 32 32 20..."])
call(["./a.out", "20", "20", "1", "32", "32", "20"])

call(["echo", "Running ./a.out 20 20 3 32 32 20..."])
call(["./a.out", "20", "20", "3", "32", "32", "20"])

call(["echo", "Running ./a.out 20 20 3 128 128 20..."])
call(["./a.out", "20", "20", "3", "128", "128", "20"])

call(["echo", "Running ./a.out 20 20 5 256 256 200..."])
call(["./a.out", "20", "20", "5", "256", "256", "200"])

call(["echo", "Running ./a.out 20 20 5 512 512 20..."])
call(["./a.out", "20", "20", "5", "512", "512", "20"])

call(["echo", "Running ./a.out 100 100 5 512 512 20..."])
call(["./a.out", "100", "100", "5", "512", "512", "20"])

call(["echo", "Running ./a.out 300 300 5 32 32 20..."])
call(["./a.out", "300", "300", "5", "32", "32", "20"])

call(["echo", "Running ./a.out 150 150 5 128 128 20..."])
call(["./a.out", "150", "150", "5", "128", "128", "20"])