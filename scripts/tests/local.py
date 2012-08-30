#coding:utf8

'''
this is doc
'''

a=1

print locals()

def f(loc):
	loc['b']=1
	b=1
	return locals()
	
print f(locals())
print locals()

if __name__=='__main__':
	pass

