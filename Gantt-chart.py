import matplotlib.pyplot as plt
process_count=0
cpu=[]
io=[]
final_cpu=[]
final_io=[]
time=0
with open ("input.txt") as inp:
    lines = inp.readlines()
    process_count=int(lines[0])
    for _ in range(process_count):
        cpu.append([])
        io.append([])
        final_cpu.append([])
        final_io.append([])
        
    for line in lines[1:]:
        time,process_id,action =  line.split(', ')
        process_id=int(process_id.split('=')[1])
        time=int(time.split('=')[1])
        action=(action.split('=')[1].replace('\n',''))
        if action == 'dispatch' or action == 'io_request' or action == 'terminate' or action == 'preept':
            cpu[process_id-1].append(time)
            if len(cpu[process_id-1])%2==0:
                one=cpu[process_id-1].pop()
                two=cpu[process_id-1].pop()
                final_cpu[process_id-1].append((two,one-two))
        elif action == 'io_givento' or action == 'io_complete':
            io[process_id-1].append(time)
            if len(io[process_id-1])%2==0:
                one=io[process_id-1].pop()
                two=io[process_id-1].pop()
                final_io[process_id-1].append((two,one-two))
        
            
fig0, gnt0 = plt.subplots()
 
fig1, gnt1 = plt.subplots()

gnt0.set_ylim(0, 100)
gnt0.set_xlim(0, time)

gnt1.set_ylim(0, 100)
gnt1.set_xlim(0, time)

gnt0.set_xlabel('Time')
gnt0.set_ylabel('Processor')

gnt1.set_xlabel('Time')
gnt1.set_ylabel('I/O')
temp=[]
for i in range(process_count):
    temp.append((i+1)*100/(process_count+1))
gnt0.set_yticks(temp)
gnt1.set_yticks(temp)
temp1=[]

for i in range(process_count):
    temp1.append('p'+str(i+1))

gnt0.set_yticklabels(temp1)
gnt1.set_yticklabels(temp1)


gnt0.grid(True)
gnt1.grid(True)


color = ['tab:blue','tab:red','tab:gray','tab:green','tab:orange','tab:purple']

for i in range(process_count):
    gnt0.broken_barh(final_cpu[i], (temp[i]-6, 12), facecolors =(color[i]))
    gnt1.broken_barh(final_io[i], (temp[i]-6, 12), facecolors =(color[i]))
 
plt.show()
