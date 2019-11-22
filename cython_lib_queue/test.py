import queue

q = queue.PyQueue()
q.push_tail('lkjhgsaf')
q.push_tail(123)
q.push_tail({'name': 'zhang', 'age': 654})
print(q.pop_tail())
print(q.pop_tail())
print(q.pop_tail())
