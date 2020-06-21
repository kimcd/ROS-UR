#! /usr/bin/env python 

import csv
import numpy 

# add error check for shape: should be a N x 7 array
# add sub-function for when no time should be imported 

def has_header(csv_file): 
    sample_bytes = 2000
    header_log = csv.Sniffer().has_header(csv_file.read(sample_bytes))
    csv_file.seek(0)
    print 'header logic:', header_log
    return header_log 

def read_goal_traj(filename):
    with open(filename) as csv_file:
        header_log = has_header(csv_file)
        csv_read = csv.reader(csv_file, delimiter=',')
	if header_log: 
	    next(csv_read)
            traj_data = numpy.array(list(csv_read), dtype=float)
            traj_t = traj_data[:,0]
            traj_q = traj_data[:,1:7]
        else: 
            traj_data = numpy.array(list(csv_read), dtype=float)
	    n_col = traj_data.shape[1]
            if n_col == 6: 
	        traj_t = []
 		traj_q = traj_data 
    	    #elif n_col == 7: 
		#traj_q = traj.data[:,1:7]
		#traj_t = traj_data[:,0]
    return traj_t, traj_q

def read_goal_traj_no_time(filename): 
    with open(filename) as csv_file:
        csv_read = csv.reader(csv_file, delimiter=',')
        next(csv_read) 
        traj_data = numpy.array(list(csv_read), dtype=float)
        traj_t = traj_data[:,0]
        traj_q = traj_data[:,1:7]
    return [traj_t, traj_q]

# def error_check(): 
# print trajectory must be a Nx7 array of time and joint values

if __name__=='__main__': 
    filename = 'episoidalslicing_slice22-8.csv'
    [traj_t, traj_q] = read_goal_traj(filename = filename)
    print traj_t
    print traj_q
