import random
import time

#! It reads empty lines as tokens

#function to choose a token based on the probabilities of the next token
def choose_token(probabilities):
    return random.choices(range(len(probabilities)), weights=probabilities, k=1)[0]
#function to remove particular characters from a string
def cleaner(string, cleanset=";_"):
    for char in cleanset:
        string = string.replace(char, "")
    return string

START = ")|)|"      #Artibitrary start token
END = "(|(|"        #Artibitrary end token
MIN = 2             #Minimum number of tokens
MAX = 100           #Maximum number of tokens
verbose = True      #Print additional information
store = True        #Store the matrix
Bot = "Yapper"      #Bot name

start = time.time()

samples = []                            #List with all the strings in the dataset

#READ THE DATASET and CLEAN IT
RAW = open("datasets/IT_dataset.txt")
for line in RAW:
    samples.append(cleaner(line.removesuffix("\n").lower()))
RAW.close()

tokenDict = {   #Starting dictionary with the start and end tokens
    START: 0,
    END: 1
}

#DICT CREATION
for sample in samples:
    tokens = sample.split(" ")
    tokens.insert(0, START)    #Add start token
    tokens.append(END)         #Add end token

    for token in tokens:
        if token in tokenDict:
            continue
        else:
            tokenDict[token] = len(tokenDict)   #Add token to dictionary, save index, for the generator

#MATRIX TRAINING
Matrix = [[0 for i in range(len(tokenDict))] for j in range(len(tokenDict))]    #Empty len(tokenDict) x len(tokenDict) matrix

for sample in samples:
    tokens = sample.split(" ")
    tokens.insert(0, START)
    tokens.append(END)

    for i in range(len(tokens) - 1):
        Matrix[tokenDict[tokens[i+1]]][tokenDict[tokens[i]]] += 1       #Count the number of times token i is followed by token i+1 in cell [i+1][i]

for c in range(len(Matrix)):            #Normalize the matrix
    if c == 1:                  #Skip the end token, 0/0 problem
        continue
    sum = 0
    for r in range(len(Matrix)):        #Sum the columns
        sum += Matrix[r][c]
    for r in range(len(Matrix)):        #Divide each column by the sum
        Matrix[r][c] /= sum


#STORE THE MATRIX or GENERATE THE TEXT
if (store):
    with open("MODELS/matrix.txt", "w") as file:
        file.write("\n")
        file.write(str(len(tokenDict)) + "\n")
        for row in Matrix:
            file.write(" ".join([str(i) for i in row]) + "\n")
        for key in tokenDict:
            file.write(key + "\n")
    if verbose:
        print("Matrix stored in MODELS/matrix.txt")
        diff = str(time.time() - start)
        print("Matrix size: "+ str(len(Matrix)))
        print("Processing time: "+ diff + "s")
        print("Samples: "+ str(len(samples)))
else:
    text = [0]
    while text[-1] != 1:
        currentCOL = [Matrix[i][text[-1]] for i in range(len(Matrix))]
        text.append(choose_token(currentCOL))
        if len(text) < MIN+1 and text[-1] == 1:
            text[-1] = random.randint(3, len(tokenDict) - 1)
        if len(text) > MAX+1:
            text.append(1)
            break

    diff = str(time.time() - start)
    if(verbose):
        print("TOKENS:", text)
    text = text[1:-1]
    text = [list(tokenDict.keys())[token] for token in text]
    print(f"{Bot}: '"+" ".join(text).capitalize(), end="'")

    if verbose:
        print("\n\nMatrix size: "+ str(len(Matrix)))
        print("Processing time: "+ diff + "s")
        print("Samples: "+ str(len(samples)))