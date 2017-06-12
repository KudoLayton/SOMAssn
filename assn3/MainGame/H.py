import random
import numpy
def genRandGrid(fInput, fOutput):
    x = random.random() * 4
    y = random.random() * 4
    fInput.write("%f %f\n" % (x, y))
    if x >= 0 and x <= 3 and y >= 0 and y <= 3:
        if x >= 1 and x <= 2:
            if y >= 1 and y <= 2:
                fOutput.write("1\n")
            else:
                fOutput.write("0\n")
        else:
            fOutput.write("1\n")
    else:
        fOutput.write("0\n")

def genRandH(fInput, fOutput):
    while True:
        x = random.random() * 4
        y = random.random() * 4
        if x >= 0 and x <= 3 and y >= 0 and y <= 3:
            if x >= 1 and x <= 2:
                if y >= 1 and y <= 2:
                    break
                else:
                    continue
            else:
                break
        else:
            continue
    
    fInput.write("%f %f\n" % (x, y))
    fOutput.write("1\n")

fInput = open("H input.txt", "w")
fOutput = open("H output.txt", "w")

fInput.write('input1 input2\n')
fOutput.write('output1\n')
for i in range(100):
    genRandH(fInput, fOutput)
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)
"""
for i in numpy.arange(0, 1, 0.1):
    fInput.write('%f %f\n' % (i, 0))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)
 
for i in numpy.arange(0, 1, 0.1):
    fInput.write('%f %f\n' % (1, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)


for i in numpy.arange(1, 2, 0.1):
    fInput.write('%f %f\n' % (1, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)


for i in numpy.arange(1, 0, 0.1):
    fInput.write('%f %f\n' % (2, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(2, 3, 0.1):
    fInput.write('%f %f\n' % (i, 0))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(0, 3, 0.1):
    fInput.write('%f %f\n' % (3, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(3, 2, 0.1):
    fInput.write('%f %f\n' % (3, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(3, 2, 0.1):
    fInput.write('%f %f\n' % (2, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(2, 1, 0.1):
    fInput.write('%f %f\n' % (i, 2))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(1, 3, 0.1):
    fInput.write('%f %f\n' % (1, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(1, 0, 0.1):
    fInput.write('%f %f\n' % (i, 3))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)

for i in numpy.arange(3, 0, 0.1):
    fInput.write('%f %f\n' % (0, i))
    fOutput.write('1\n')
    genRandGrid(fInput, fOutput)
    genRandGrid(fInput, fOutput)
"""
fInput.close()
fOutput.close()

