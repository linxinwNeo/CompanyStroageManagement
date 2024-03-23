#coding=utf-8

import math

split_item = '&&'
                
def write2txt(models, path) :
    with open(path, 'w', encoding="utf-8") as file:
        # Write content to the file
        
        # write the number of models
        file.write('% ' + str(len(models)) + split_item + '0 \n')
        file.write('% MODEL_CODE DESCRIPTION_SPAN DESCRIPTION_CN PRIZE_PER_PIECE NUM_INITIAL_PIECES NUM_SOLD_PIECES NUM_LEFT_PIECES NUM_PIECES_PER_BOX CONTAINER_ID\n')
        for key, value in models.items():
            file.write(value.MODEL_CODE + split_item)
            file.write(value.DESCRIPTION_SPAN + split_item)
            if value.DESCRIPTION_CN is not None:
                file.write(value.DESCRIPTION_CN + split_item)
            else:
                file.write(split_item)
            file.write(str(value.PRIZE) + split_item)
            
            num_init_pieces = math.floor(value.NUM_INITIAL_BOXES * value.NUM_ITEMS_PER_BOX)
            file.write(str(num_init_pieces) + split_item)

            num_sold_pieces = math.floor(value.NUM_SOLD_BOXES * value.NUM_ITEMS_PER_BOX)
            file.write(str( num_sold_pieces) + split_item)

            file.write(str(num_init_pieces - num_sold_pieces) + split_item)
            file.write(str(value.NUM_ITEMS_PER_BOX) + split_item)
            file.write( '-1' ) # default container is -1
            file.write( ' \n' )
        file.close()
    return
