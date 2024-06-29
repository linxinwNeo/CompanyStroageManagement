# -*- coding: utf-8 -*-
from readInventario import read_inventory_file
from readPrizes import read_Prizes_file
from save2txt import write2txt

inventory_file_path = 'C:/Users/Neo/Desktop/GithubProjs/CompanyStroageManagement/pythonFiles/raw_data_before_processing/inventory.xlsx'
prizes_file_path = 'C:/Users/Neo/Desktop/GithubProjs/CompanyStroageManagement/pythonFiles/raw_data_before_processing/prizes.xlsx'

# a dict of models
models = {}

read_inventory_file(inventory_file_path, models)
read_Prizes_file(prizes_file_path, models)

# print the items
# for key, value in models.items():
#     value.print()

write2txt(models, "C:/Users/Neo/Desktop/inventory.txt")