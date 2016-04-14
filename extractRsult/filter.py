

with open('result', 'r') as read, open('finalResult', 'w') as write:
	line = read.readline()
	while line:
		if  line == "\\2-grams:\n":
			break
		else:
			line = read.readline()
	line = read.readline().strip('\n')
	while line:
		content = line.split('	')
		if len(content) == 2:
			word = content[1].split(' ')
			if word[0] != '<s>' and word[1] != '</s>':
				if word[0] > word[1]:
					word[0],word[1] = word[1], word[0];
				write.write('%s %s	%s\n' % (word[0],word[1],content[0]))
		line = read.readline().strip('\n')
read.close()
write.close()