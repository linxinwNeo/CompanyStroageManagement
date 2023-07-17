import re

class Model:
    def __init__(self):
        self.MODEL_CODE = '' # 货号， string
        self.DESCRIPTION_SPAN = '' # 名称 英文 string
        self.DESCRIPTION_CN = '' # 名称 中文 string
        self.PRIZE = 0. # 价格， 浮点数
        self.NUM_INITIAL_BOXES = 0. # 进货箱数，浮点数
        self.NUM_SOLD_BOXES = 0. # 卖出箱数，浮点数
        self.NUM_LEFT_ITEMS = 0 # 剩余件数，整数
        self.NUM_LEFT_BOXES = 0. # 剩余箱数，浮点数
        self.NUM_ITEMS_PER_BOX = 0. # 每箱件数，整数
    
    def print(self):
        print("MODEL_CODE " + self.MODEL_CODE)
        print("DESCRIPTION_SPAN " + self.DESCRIPTION_SPAN )
        print("DESCRIPTION_CN " + remove_non_alnum_hyphen(self.DESCRIPTION_CN))
        print("PRIZE " + str(self.PRIZE))
        print("NUM_INITIAL_BOXES " + str(self.NUM_INITIAL_BOXES))
        print("NUM_SOLD_BOXES " + str(self.NUM_SOLD_BOXES))
        print("NUM_LEFT_ITEMS " + str(self.NUM_LEFT_ITEMS))
        print("NUM_LEFT_BOXES " + str(self.NUM_LEFT_BOXES))
        print("NUM_ITEMS_PER_BOX " + str(self.NUM_ITEMS_PER_BOX))

def remove_non_alnum_hyphen(string):
    if string is None:
        return string
    pattern = r'[^a-zA-Z0-9-]'  # Regular expression pattern to match non-alphanumeric characters and hyphens
    return re.sub(pattern, '', string)

def remove_lineSwitcher(string):
    if string is None:
        return string
    new_str = string.replace("\n", " ")
    new_str = new_str.replace("\r", " ")
    return new_str