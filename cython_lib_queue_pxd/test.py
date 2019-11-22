import queue

q = queue.Queue()

q.push_tail('huhu')
q.push_tail(56645)
q.push_tail({'name': 'liu', 'age': 50})
print(q.pop_tail())
print(q.pop_tail())
print(q.pop_tail())
