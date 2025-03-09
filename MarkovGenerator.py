import random
import time

#function to choose a token based on the probabilities of the next token
def choose_token(probabilities):
    return random.choices(range(len(probabilities)), weights=probabilities, k=1)[0]

start = time.time()
MIN = 5         #Minimum number of tokens
MAX = 100       #Maximum number of tokens
size = 0        #Matrix size
verbose = False  #Print additional information
tokenDict = {} #Dictionary
Matrix = []     #Matrix
Bot = "Yapper"  #Bot name

#TODO ADD the reading of matrix.txt
with open("MODELS/matrix.txt") as f:
    lineCounter = 0
    for line in f:
        if lineCounter == 0:
            size = int(f.readline())
            lineCounter += 1
        for i in range(size):
            Matrix.append(list(map(float, f.readline().split())))
        token_counter = 0
        for i in range(size):
            tokenDict[f.readline().removesuffix("\n")] = token_counter
            token_counter += 1

#text generation
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
    print("Samples: "+ str(size))