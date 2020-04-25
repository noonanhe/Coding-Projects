import simplejson as json
import sys
from sklearn.metrics import roc_curve, auc, roc_auc_score
from sklearn.metrics import precision_recall_curve
import re


def removeBranchLen(tree):
    numList = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ":"]
    flag = 0
    temtree = ""
    for i in tree.strip():
        if i == ":":
            flag = 1
        if (flag == 1) and (i not in numList):
            flag = 0
        if flag == 0:
            temtree += i
    return temtree


def treeNodes(tree):
    cleanTree = tree.replace('(', '').replace(')', '').replace(';', '')
    # remove "(", ")", ";"
    cleanTree = re.sub(r'\[+.*?\]+', '', cleanTree)
    # remove [***]
    return cleanTree.strip().split(',')


def findBiPartitons(tree, withSupport):
    allNodes = treeNodes(tree)
    bipartition = {}

    for i, c in enumerate(tree):
        if c == ')' and tree[i + 1] != ';':
            # find bipartitions
            rawnodes = ''
            stack = list(tree[:i])
            needed = 1
            while stack:
                if stack[-1] == ')':
                    needed += 1
                elif stack[-1] == '(':
                    needed -= 1
                if needed == 0:
                    break
                rawnodes += stack.pop()
            rawnodes = rawnodes[::-1]
            b1 = treeNodes(rawnodes)
            b2 = [x for x in allNodes if x not in b1]

            support = -1
            if withSupport and tree[i + 1] == '[':
                # find support
                support = ''
                temindex = i + 2
                while temindex < len(tree) and tree[temindex] != ']':
                    support += tree[temindex]
                    temindex += 1
                if support.isdigit():
                    support = float(support) / 100.

            b1.sort()
            b2.sort()
            b1str = ','.join(b1)
            b2str = ','.join(b2)
            bi = b1str + '|' + b2str
            bipartition[bi] = support
    return bipartition


def readMultipleTrees(infile):
    trees = list()
    with open(infile, 'r') as inf:
        for line in inf.readlines():
            if line[0] == '(':
                tree = line.strip()
            trees.append(tree)
    return trees

def readTree(infile):
    with open(infile, 'r') as inf:
        for line in inf.readlines():
            if line[0] == '(':
                tree = line.strip()
    return tree

def histCounts(values):
    counts = {20: 0, 40: 0, 60: 0, 80:0, 100:0}
    for num in values:
        if num < 20:
            counts[20] += 1
        elif num < 40:
            counts[40] += 1
        elif num < 60:
            counts[60] +=1
        elif num<80:
            counts[80] += 1
        else:
            counts[100] += 1
    return (counts)



#lets read the tree file
#tree_list = readTree("rawr.trees")
treeFile = sys.argv[1]
modelFile = sys.argv[2]
inferFile = sys.argv[3]
tree_list = readMultipleTrees(treeFile)

for i in range(len(tree_list)):
    tree_list[i] = removeBranchLen(tree_list[i])
tree_listbi = list()
for i in range(len(tree_list)):
    tree_listbi.append(findBiPartitons(tree_list[i], 0))

branches = {}
for t in tree_listbi:
    for b in t:
        b1 = b
        b2 = '|'.join(b.split('|')[::-1])
        if b1 not in branches:
            if b2 not in branches:
                branches[b1] = 1
            else:
                branches[b2] += 1
        else:
            branches[b1] += 1

print(branches)
print("Just Values:")
print(branches.values())
print("For Histogram:")
print(histCounts(branches.values()))
print("Model Tree:")
model = readTree(modelFile)
model = removeBranchLen(model)
model_bi = findBiPartitons(model, 0)
model_branches = {}
for b in model_bi:
    b1 = b
    b2 = '|'.join(b.split('|')[::-1])
    if b1 not in model_branches:
        if b2 not in model_branches:
            model_branches[b1] = 1
        else:
            model_branches[b2] += 1
    else:
        model_branches[b1] += 1
print(model_branches)
#print("Model in Reps:")
#both1 = [0]*len(branches)
# index = 0
# for b in branches:
#    if b in model_branches:
#        both1[index] = 1
#    index += 1
#print(both1)
print("Reps in Model:")
both2 = [0]*len(model_branches)
index =0
for b in model_branches:
    if b in branches:
        both2[index] = branches[b]
    index += 1
print(both2)
print("For Histogram:")
print(histCounts(both2))

infer = readTree(inferFile)
infer = removeBranchLen(infer)
infer_bi = findBiPartitons(infer, 0)
infer_branches = {}
for b in infer_bi:
    b1 = b
    b2 = '|'.join(b.split('|')[::-1])
    if b1 not in infer_branches:
        if b2 not in infer_branches:
            infer_branches[b1] = 1
        else:
            infer_branches[b2] += 1
    else:
        infer_branches[b1] += 1
print(infer_branches)
#print("Infer in Reps:")
#both3 = [0]*len(branches)
#index =0
#for b in branches:
#    if b in infer_branches:
#        both3[index] = 1
#    index += 1
#print(both3)
#print("Reps in Model:")
print("Reps in Infer")
both4 = [0]*len(infer_branches)
index = 0
for b in infer_branches:
    if b in branches:
        both4[index] = branches[b]
    index += 1

print(both4)
print("For Histogram:")
print(histCounts(both4))
print("Histogram for Both:")
both5 = {}
for b in model_branches:

    if b in infer_branches:
        both5[b] = branches[b]
print(both5)
