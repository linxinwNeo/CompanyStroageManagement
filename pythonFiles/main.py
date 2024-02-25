# -*- coding: utf-8 -*-
from readInventario import read_inventory_file
from readPrizes import read_Prizes_file
from save2txt import write2txt

inventory_file_path = 'C:/Users/linxi/OneDrive/Documents/GitHub/CompanyStroageManagement/inventory.xlsx'
prizes_file_path = 'C:/Users/linxi/OneDrive/Documents/GitHub/CompanyStroageManagement/prizes.xlsx'


# a dict of models
models = {}

read_inventory_file(inventory_file_path, models)
read_Prizes_file(prizes_file_path, models)

# print the items
# for key, value in models.items():
#     value.print()

write2txt(models, "C:/Users/linxi/OneDrive/Desktop/inventory.txt")