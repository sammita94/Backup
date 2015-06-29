#!/usr/bin/python

import sys
import random
import os
from math import sqrt
import difflib
from random import randint


if len(sys.argv) != 10:
	print("\nUSAGE :: <<Program>> <<Total no. paths>> <<clients per path>> <<speed in km/hr>> <<AP file>> <<input all_path filename>> <<line string file>> <<Output File>> <<min Path Length>> <<max Path Length>>\n\n")
	exit(0)

os.system("rm -rf temp.txt AP_path.txt client_paths.txt")

total_paths		= int(sys.argv[1])
clients_per_path	= int(sys.argv[2])
speed_in_km		= int(sys.argv[3])
ap_file                 = sys.argv[4]
all_path_file           = sys.argv[5]
line_string_file        = sys.argv[6]
output_file             = sys.argv[7]
min_path_len            = int(sys.argv[8])
max_path_len            = int(sys.argv[9])

ap_file_handle               = open(ap_file,'r')
ap_file_content              = ap_file_handle.readlines()
ap_file_handle.close()

all_path_file_handle               = open(all_path_file,'r')
all_path_file_content              = all_path_file_handle.readlines()
all_path_file_handle.close()

line_string_file_handle               = open(line_string_file,'r')
line_string_file_content              = line_string_file_handle.readlines()
line_string_file_handle.close()

out_handle              	 = open("temp.txt",'w')
client_path_handle               = open("client_paths.txt",'w')

ap_list_dict	= {}
line_string_dict = {}
count = 1
for line in ap_file_content:
	lineList = line.strip().split(' ')
	temp_coord_dict ={}
	temp_coord = str(lineList[0] + ' ' + lineList[1])
	temp_coord_dict[temp_coord] = lineList[2:]
	ap_list_dict[count] = temp_coord_dict
	count = count + 1

#print ap_list_dict 
for line in line_string_file_content:	
	lineList = line.strip().split(':')
	key = lineList[0]
	line_string_dict[key] = lineList[1].strip().split(',')

print line_string_dict

random_dict = {}

all_path_len = len(all_path_file_content)
#print " all = " + str(all_path_len)  + "\n"
count = 0

while count != total_paths:
	rand_num = randint(1,all_path_len)

	if rand_num not in random_dict:
		if (len(all_path_file_content[rand_num - 1].strip().split(' ')) >= min_path_len ) and (len(all_path_file_content[rand_num - 1].strip().split(' ')) <= max_path_len ):
			#print len(all_path_file_content[rand_num - 1].strip().split(' '))
			#print all_path_file_content[rand_num - 1] 
			random_dict[rand_num] = 1
			count += 1

print random_dict

#Small Intersection Routine
def intersect(a, b):
     return list(set(a) & set(b))

client_id = 0

for element in random_dict:
	selected_path_list = all_path_file_content[int(element) - 1].strip().split(' ');
	print selected_path_list
	
	path_str = ""
	for cl in range(len(selected_path_list) - 1):
		path_str = path_str + str(selected_path_list[cl]) + " "
	path_str = path_str + selected_path_list[len(selected_path_list) - 1]
	
	copy= int(element)
	client_path_handle.writelines(str(client_id) + " " + str(copy) + " " + str(len(selected_path_list)) + " " + str(path_str) +"\n") 
	
	#print ap_list_dict
	startTime = random.random()*2.0
	lastTime = startTime

	for i in range(len(selected_path_list) - 1):
		ap_1 = int(selected_path_list[i])
		ap_2 = int(selected_path_list[i+1])

		ap_1_list = ap_list_dict[ap_1].values()[0]
		ap_2_list = ap_list_dict[ap_2].values()[0]

		ap_1_coord = ap_list_dict[ap_1].keys()[0]
		ap_2_coord = ap_list_dict[ap_2].keys()[0]

		intrs_List = intersect(ap_1_list,ap_2_list)
		#print ap_1_coord
		#print ap_2_coord
		
		cord_list = line_string_dict[ intrs_List[0] ]
		#print cord_list

		ap_1_index =  cord_list.index(ap_1_coord)
		ap_2_index =  cord_list.index(ap_2_coord) 

		incr = 0
		incr_count = 0

		if ap_1_index > ap_2_index:
			incr = -1
			incr_count = ap_1_index - ap_2_index
		else:
			incr = 1
			incr_count = ap_2_index - ap_1_index

		last_co_ordinate = ap_1_coord
		if i == 0:
			start_index = 0
		else:
			start_index = 1

		for j in range(start_index,incr_count+1):
			co_ord_index = ap_1_index + incr*j
			cur_co_ordinate = cord_list[co_ord_index]

			last_x = last_co_ordinate.strip().split(' ')[0]
			last_y = last_co_ordinate.strip().split(' ')[1]

			cur_x = cur_co_ordinate.strip().split(' ')[0]
			cur_y = cur_co_ordinate.strip().split(' ')[1]

			dist = sqrt( (float(cur_x) - float(last_x))*(float(cur_x) - float(last_x)) + (float(cur_y) - float(last_y))*(float(cur_y) - float(last_y)))
			dtime = float(float(dist*3600) / float(speed_in_km*1000))
			
			cur_time = float(float(lastTime) + dtime)
			lastTime = cur_time
			last_co_ordinate = cur_co_ordinate
		
			out_handle.writelines(str(cur_time) + " " + str(client_id) + " " + cur_co_ordinate + " " + str(speed_in_km) +"\n");
			os.system("echo "+str(cur_time) + " " + str(client_id) + " " + cur_co_ordinate + " " + str(speed_in_km)+">>"+ "client_"+str(client_id)+".txt")

	client_id += 1

out_handle.close();
client_path_handle.close();

os.system("sort -n temp.txt > " + sys.argv[7]);		
