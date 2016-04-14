import sys

#find the minimum two word in the line
def sort(new_dict, query):
	#first_minimum, second_minimum = twoSort(new_dict, query[0], query[1])
	first_minimum = None
	first_minimum_value = sys.maxint
	second_minimum = None
	second_minimum_value = sys.maxint
	for i in range(0, len(query)):
		if new_dict.get(query[i]) == None:
			continue;
		if int(new_dict.get(query[i])) < first_minimum_value:
			second_minimum = first_minimum
			second_minimum_value =first_minimum_value
			first_minimum = query[i]
			first_minimum_value = int(new_dict.get(query[i]))
		else:
			if int(new_dict.get(query[i])) < second_minimum_value:
				second_minimum = query[i]
				second_minimum_value = int(new_dict.get(query[i]))
	return first_minimum, second_minimum

#sort the first two element in the line
# def twoSort(new_dict, key1, key2):
# 	if new_dict.get(key1) < new_dict.get(key2):
# 		first_minimum = key1
# 		second_minimum = key2
# 	else:
# 		first_minimum = key2
# 		second_minimum = key1
# 	return first_minimum, second_minimum

#store the lecikon data in the dictionary
d = {}
with open("lexikon.txt") as lexikon:
	for line in lexikon:
		key  = line.split(' ')[0]
		value = line.split(' ')[2]
		d[key] = value

# The temp file is to store the lexikon data
# just for test use
temp = open('temp','w')
for key in d:
	temp.write(key+' ')
	temp.write(d.get(key))
	temp.write('\n')
lexikon.close()

# open the TestQuery to get the unique term
new_dict = dict()
with open('trainQuery', 'r') as querySet:
	for line in querySet:
		query = line.strip('\n').split(' ')
		for term in query:
			# if term == 'sound':
			# 	print d.get('sound')
			if not new_dict.has_key(term):
				new_dict[term] = d.get(term);
querySet.close()

with open('trainQuery','r') as querySet, open('train','w') as train:
	for line in querySet:
		formalLine = line.strip('\n').strip(' ').lstrip(' ')
		query = formalLine.split(' ')
		if len(query) < 2:
			continue
		else:
			first_minimum, second_minimum = sort(new_dict, query)
			if second_minimum != None:
				train.write(first_minimum + ' ' + second_minimum + '\n')
querySet.close()
train.close()
# generate a small lexikon txt file
unique = open('UniqueTerm','w')
for key in new_dict: 
	unique.write(key+' ')
	unique.write(str(new_dict.get(key)))
	unique.write('\n')
unique.close();





