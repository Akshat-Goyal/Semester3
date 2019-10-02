from collections import OrderedDict
import json, jsbeautifier


def finalState(l, r, x, y, Y, arr):
	if l > r and y == 1:
		arr.append(x)
		return arr
	elif l > r:
		return arr

	arr = finalState(l+1, r, x, y, Y, arr)
	if ((1<<l)&Y) != 0:
		arr = finalState(l+1, r, x+(1<<l), 1, Y, arr)
	else:
		arr = finalState(l+1, r, x+(1<<l), y, Y, arr)
	return arr


def t_func(DFA, NFA, L):
	for i in range(DFA["states"]):				
		for j in range(L):
			DFA["t_func"][L*i+j] = [i, DFA["letters"][j], 0]

	for i in NFA["t_func"]:						
		x = 0
		for j in i[2]:
			x = x+(1<<j)
		DFA["t_func"][L*(1<<i[0]) + DFA["letters"].index(i[1])] = [1<<i[0], i[1], x]

	for i in range(DFA["states"]):
		for j in range(L):
			temp = i
			n = 0
			X = 0
			while temp!=0: 
				if temp%2 == 1:
					x = DFA["t_func"][L*(1<<n)+j][2]
					X = X|x
				temp = temp>>1 
				n = n+1

			DFA["t_func"][L*i+j] = [i, DFA["letters"][j], X]

	return DFA["t_func"]



if __name__ == '__main__':

	with open('./input.json') as f:
		NFA = json.load(f, object_pairs_hook = OrderedDict)

	DFA = OrderedDict()
	DFA["states"] = 2**NFA["states"]
	DFA["letters"] = NFA["letters"]
	DFA["t_func"] = [0]*(DFA["states"]*len(DFA["letters"]))
	DFA["start"] = 2**NFA["start"]
	DFA["final"] = list()
								
	Y = 0											
	for i in NFA["final"]:
		Y = Y + (1<<i)

	DFA["final"] = finalState(0, NFA["states"]-1, 0, 0, Y, DFA["final"])							
	DFA["t_func"] = t_func(DFA, NFA, len(DFA["letters"]))

	opts = jsbeautifier.default_options()
	opts.indent_size = 2
	formatted_json = jsbeautifier.beautify(json.dumps(DFA), opts)

	with open('./output.json', 'w') as json_file:
		json_file.write(formatted_json)