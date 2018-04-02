# -*- coding: utf-8 -*-

from __future__ import print_function, division

import argparse
import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import lr_scheduler
from torch.autograd import Variable
import numpy as np
import torchvision
from torchvision import datasets, models, transforms
import time
import os
import scipy.io
from model import ft_net 
from xml.dom.minidom import Document

######################################################################
# Load model
#---------------------------
def load_network(network):
    save_path = os.path.join('./model',name,'net_%s.pth'%opt.which_epoch)
    network.load_state_dict(torch.load(save_path))
    return network

######################################################################
# Extract feature
# ----------------------
# Extract feature from  a trained model.
#
def fliplr(img):
    #flip horizontal
	inv_idx = torch.arange(img.size(3)-1,-1,-1).long()
	if use_gpu:
		inv_idx = inv_idx.cuda()
	img_flip = img.index_select(3,inv_idx)

	return img_flip
 
def extract_feature(model,dataloaders):
    features = torch.FloatTensor()
    count = 0
    for data in dataloaders:
        img, label = data
        n, c, h, w = img.size()
        count += n
        print(count)
        ff = torch.FloatTensor(n,2048).zero_()

        for i in range(2):    
            if(i==1):
                img = fliplr(img)
            if use_gpu:
            	img = img.cuda()
            input_img = Variable(img)
            outputs = model(input_img)
            f = outputs.data.cpu()
            ff = ff+f
        # norm feature
        fnorm = torch.norm(ff, p=2, dim=1, keepdim=True)
        ff = ff.div(fnorm.expand_as(ff))
        features = torch.cat((features,ff), 0)
    return features

def get_name(img_path):
    names = []
    for path, v in img_path:
        filename = os.path.basename(path)
        name = filename.split('.')[0]
        names.append(name)
    return names

#######################################################################
# Evaluate
def evaluate(qf, qn, gf, gn):
    score = np.dot(gf,qf)

    sorted_list = []
    for i in range(0,len(score)):
        sorted_list.append((score[i], gn[i]))

    sorted_list.sort()
    sorted_list.reverse()

    result_name = []
    for i in range(0, len(sorted_list)):
        score, gn = sorted_list[i]
        result_name.append(gn)

    return result_name #a list of sorted result (from large to small)

def writeInfoToXML(query_names, all_results, write_path):
    #creat document
    doc = Document()
    #creat root
    message = doc.createElement('Message')
    message.setAttribute('Version', '1.0')

    doc.appendChild(message)

    items = doc.createElement('Items')
    message.appendChild(items)

    for i in range(0, len(query_names)):
        #get current Item value
        query = query_names[i].split('/')[-1]
        result = all_results[i]
        #creat Item
        item = doc.createElement('Item')
        item.setAttribute('imageName',query)
        #store sorted reference data result
        for j in range(0, len(result)):
            if j == 0:
                refer_str = result[j]
            else:
                refer_str += ' ' + result[j]

        show_text = doc.createTextNode(refer_str)
        item.appendChild(show_text)

        items.appendChild(item)

    f = open(write_path, 'w')
    f.write(doc.toprettyxml(indent='\t'))
    f.close()

if __name__ == '__main__':

######################################################################
# Options
	parser = argparse.ArgumentParser(description='Training')
	parser.add_argument('--gpu_ids',default='0', type=str,help='gpu_ids: e.g. 0  0,1,2  0,2')
	parser.add_argument('--which_epoch',default='last', type=str, help='0,1,2,3...or last')
	parser.add_argument('--query_dir',default='dataset/test_query',type=str, help='./test_data')
	parser.add_argument('--name', default='ft_ResNet50', type=str, help='save model path')
	parser.add_argument('--batchsize', default=1, type=int, help='batchsize')

# read options from input
	opt = parser.parse_args()

	str_ids = opt.gpu_ids.split(',')
#which_epoch = opt.which_epoch
	name = opt.name
	query_dir = opt.query_dir

# set gpu
	use_gpu = torch.cuda.is_available()
	if use_gpu:
		gpu_ids = []
		for str_id in str_ids:
			id = int(str_id)
			if id >=0:
				gpu_ids.append(id)

		if len(gpu_ids)>0:
			torch.cuda.set_device(gpu_ids[0])


######################################################################
# Load Data
# ---------
# use torchvision and torch.utils.data packages for loading the data
#
	data_transforms = transforms.Compose([
        transforms.Resize((288,144), interpolation=3),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
	])

	image_datasets = datasets.ImageFolder(query_dir, data_transforms)
	dataloaders = torch.utils.data.DataLoader(image_datasets, batch_size=opt.batchsize,
                                             shuffle=False, num_workers=4)

	query_path = image_datasets.imgs
	query_name = get_name(query_path)

######################################################################
# Load Collected data Trained model
	print('-------test-----------')
	model_structure = ft_net(751)
	model = load_network(model_structure)

# Remove the final fc layer and classifier layer
	model.model.fc = nn.Sequential()
	model.classifier = nn.Sequential()

# Change to test mode
	model = model.eval()
	if use_gpu:
		model = model.cuda()

#######################################################################
# Extract feature
	print('__________________extract features of query___________________')
	query_feature = extract_feature(model,dataloaders).numpy()

# Save to Matlab for check
	result = {'query_f':query_feature,'query_name':query_name}
	scipy.io.savemat('feature/query_result.mat',result)

############################################################
# load features of reference
	print('____________________load features of reference_________________')
	gallery = scipy.io.loadmat('feature/gallery_feature.mat')
	gallery_feature = gallery['gallery_f']
	gallery_name = gallery['gallery_name']

#############################################################
# evaluate
	print('______________________evaluate_____________________________')
	all_results = []
	for i in range(len(query_name)):
		message = ''
		result_name = evaluate(query_feature[i],query_name[i],gallery_feature,gallery_name)
		all_results.append(result_name)

	print('____________________write xml____________________________')
	write_path = 'result_XML/result.xml'
	if not os.path.exists('result_XML/'):
		os.makedirs('result_XML/')
	writeInfoToXML(query_name, all_results, write_path)
