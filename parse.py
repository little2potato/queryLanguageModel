def buildMap():
	stopList = file('stoplist.txt')
	term = []
	for line in stopList:
		term.append(line.split('\n')[0])
	return term

def check(item, diction):
	if item in diction:
		return False
 	else:
 		return True

diction = buildMap()
with open('smallQuerySet', 'r') as querySet, open('testQuery', 'w') as parsed:
	line = querySet.readline().strip('\n')
	while line:
		query = line.split(':')[1].split(' ')
		for item in query:
			if check(item, diction):
				parsed.write(item)
				parsed.write(' ')
		parsed.write('\n')
		line = querySet.readline().strip('\n')
querySet.close()
parsed.close()