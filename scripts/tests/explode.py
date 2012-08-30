#coding:utf8

'''
this is doc
'''

a=1

print locals()
print locals().update({'b':2, 'a':3})
print locals()

def export(outer_namespace, varibles_dict):
	outer_namespace.update(varibles_dict)
	
export(locals(), {'a':4})
print locals()

if __name__=='__main__':
	pass

