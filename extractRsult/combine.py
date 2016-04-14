import math

with open('sortedResult', 'r') as read, open('combinedResult', 'w') as write:
	line = read.readline().strip('\n')
	former = None
	while line:
		if former == None:
			former = line
		else:
			formerContent = former.split('	')
			lineContent = line.split('	')
			if formerContent[0] == lineContent[0]:
				print "exists duplicate"
				print formerContent
				formerProb = pow(2,float(lineContent[1]))
				lineProb = pow(2,float(lineContent[1]))
				sumProb = math.log(formerProb + lineProb,2)
				formerContent[1] = str(sumProb)
				former = formerContent[0]+'	'+formerContent[1]
			else:
				write.write('%s\n' % former)	
				former = line
		line = read.readline().strip('\n')
	write.write('%s\n' % former)
read.close()
write.close()