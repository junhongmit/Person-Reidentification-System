# -*- coding: utf-8 -*-

from __future__ import print_function, division

import argparse
import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import lr_scheduler
from torch.autograd import Variable
import numpy as np
#import torchvision
from torchvision import transforms
import time
import os
import scipy.io
from model import ft_net 
import shutil
from PIL import Image

######################################################################
# get features of a single picture
def fliplr(img):
    '''flip horizontal'''
    inv_idx = torch.arange(img.size(3)-1,-1,-1).long()  # N x C x H x W
    img_flip = img.index_select(3,inv_idx)
    return img_flip

def single_feature(model, img):
	ff = torch.FloatTensor(1,2048).zero_()
	for i in range(2):
		if(i==1):
			img = fliplr(img)
		#input_img = Variable(img.cuda())
		input_img = Variable(img)
		if use_gpu:
			outputs = model(input_img.cuda())
		else:
			outputs = model(input_img) 
		f = outputs.data.cpu()
		#print(f.size())
		ff = ff+f
	# norm feature
	fnorm = torch.norm(ff, p=2, dim=1, keepdim=True)
	ff = ff.div(fnorm.expand_as(ff)).numpy()
	return np.transpose(ff)

#######################################################################
# Evaluate and return a sorted list of reference
def evaluate(qf, gf, gn):
    #print(np.shape(gf))
    #print(np.shape(qf))
    score = np.dot(gf,qf)

    sorted_list = []
    for i in range(0,len(score)):
        sorted_list.append((score[i], gn[i]))

    sorted_list.sort()
    sorted_list.reverse()

    print_matching_rate(sorted_list)

    result_name = []
    for i in range(0, len(sorted_list)):
        score, gn = sorted_list[i]
        result_name.append(gn)



    return result_name #a list of sorted result (from large to small)

#######################################################################
# print top15 matching rate
def print_matching_rate(sorted_list):
	for i in range(0, 15):
		score, gn = sorted_list[i]
		print('Top ' + str(i+1) + '\t# ID: ' + gn + ', matching rate: ' + str(float(score*100)) + '%')

#######################################################################
# Save Top15 picture
def save_top15(pic_path, result_name):
    pic_name = os.path.basename(pic_path)
    pic_num = pic_name.split('.')[0]
    save_dir = 'result_JPG/' + pic_num + '/'
    #ref_dir = test_dir + 'gallery/-1/'
    
    if not os.path.exists('result_JPG/'):
        os.makedirs('result_JPG/')
        
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    #shutil.copyfile(pic_path,  save_dir + '0_' + pic_name)

    for i in range(0, 15):
        filename = str(i+1) + '_' + result_name[i] + '.jpg'
        shutil.copyfile(ref_dir + result_name[i] + '.jpg',  save_dir + filename)

######################################################################
# Load model
#---------------------------
def load_network(network):
    save_path = os.path.join('./model',name,'net_%s.pth'%opt.which_epoch)
    network.load_state_dict(torch.load(save_path))
    return network

######################################################################
# Options
# --------
# example, run this in ubuntu : 
# python test.py --gpu_ids 0 --name ft_ResNet50 --test_dir /home/zhoupan/disk1/Market-1501-v15.09.15/pytorch/  --which_epoch 59

parser = argparse.ArgumentParser(description='Training')
parser.add_argument('--gpu_ids',default='0', type=str,help='gpu_ids: e.g. 0  0,1,2  0,2')
parser.add_argument('--ref_dir',default='dataset/test_reference/',type=str, help='./test_data')
parser.add_argument('--which_epoch',default='last', type=str, help='0,1,2,3...or last')
parser.add_argument('--name', default='ft_ResNet50', type=str, help='save model path')
parser.add_argument('--batchsize', default=1, type=int, help='batchsize')
parser.add_argument('--pic_path', default='dataset/test_query/-1/015637.jpg', type=str, help='picture path')

# read options from input
opt = parser.parse_args()

str_ids = opt.gpu_ids.split(',')
#which_epoch = opt.which_epoch
name = opt.name
ref_dir = opt.ref_dir
pic_path = opt.pic_path

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
# Load Collected data Trained model
print('loading model......')
model_structure = ft_net(751)
model = load_network(model_structure)

# Remove the final fc layer and classifier layer
model.model.fc = nn.Sequential()
model.classifier = nn.Sequential()

# Change to test mode
model = model.eval()
if use_gpu:
    model = model.cuda()
print('model loaded!')

######################################################################
# Load Data
print('loading data......')
data_transforms = transforms.Compose([
        transforms.Resize((288,144), interpolation=3),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

# get features of the picture
img = Image.open(pic_path).convert('RGB')
img = data_transforms(img)
c, h, w = img.size()
#print(img.size())
new_img = torch.zeros(1,c,h,w)
new_img[0] = img
img_feature = single_feature(model, new_img)

# load features of reference 
result = scipy.io.loadmat('feature/gallery_feature.mat')
gallery_feature = result['gallery_f']
gallery_name = result['gallery_name']
print('data loaded!')

######################################################################
# evaluate 
# get a sorted list of reference
print('----------------------------------------')
print('start evaluating...')
result_name = evaluate(img_feature, gallery_feature, gallery_name)
print('evaluation done!')

save_top15(pic_path, result_name)