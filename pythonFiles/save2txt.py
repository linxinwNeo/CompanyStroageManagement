#coding=utf-8

import math
from datetime import datetime

split_item = '&&'
                
def write2txt(models, path) :
    current_time = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')


    with open(path, 'w', encoding="utf-8") as file:
        # Write content to the file
        
        # write the number of models
        file.write('% ' + str(len(models)) + split_item + ' \n')
        file.write('% MODEL_CODE CONTAINER_ID DESCRIPTION_CN DESCRIPTION_SPAN NUM_INITIAL_PIECES NUM_SOLD_PIECES NUM_LEFT_PIECES NUM_PIECES_PER_BOX PRIZE_PER_PIECE TIME_MODIFIED \n')
        for key, value in models.items():
            file.write(value.MODEL_CODE + split_item)
            file.write( '-1' + split_item) # default container is -1

            if value.DESCRIPTION_CN is not None:
                file.write(value.DESCRIPTION_CN + split_item)
            else:
                file.write(split_item)

            file.write(value.DESCRIPTION_SPAN + split_item)
            
            num_init_pieces = math.floor(value.NUM_INITIAL_BOXES * value.NUM_ITEMS_PER_BOX)
            file.write(str(num_init_pieces) + split_item)

            num_sold_pieces = math.floor(value.NUM_SOLD_BOXES * value.NUM_ITEMS_PER_BOX)
            file.write(str(num_sold_pieces) + split_item)

            file.write(str(num_init_pieces - num_sold_pieces) + split_item)
            file.write(str(value.NUM_ITEMS_PER_BOX) + split_item)

            file.write(str(value.PRIZE) + split_item)

            file.write(current_time + split_item)
            
            file.write( ' \n' )
        file.close()
    return
