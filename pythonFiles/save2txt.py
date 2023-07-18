#coding=utf-8
split_item = '&&'
                
def write2txt(models, path) :
    with open(path, 'w', encoding="utf-8") as file:
        # Write content to the file
        
        # write the number of models
        file.write('% ' + str(len(models)) + split_item + '0 \n')
        file.write('% MODEL_CODE DESCRIPTION_SPAN DESCRIPTION_CN PRIZE NUM_INITIAL_BOXES NUM_LEFT_ITEMS NUM_LEFT_BOXES NUM_ITEMS_PER_BOX\n')
        for key, value in models.items():
            file.write(value.MODEL_CODE + split_item)
            file.write(value.DESCRIPTION_SPAN + split_item)
            if value.DESCRIPTION_CN is not None:
                file.write(value.DESCRIPTION_CN + split_item)
            else:
                file.write(split_item)
            file.write(str(value.PRIZE) + split_item)
            file.write(str(value.NUM_INITIAL_BOXES) + split_item)
            file.write(str(value.NUM_SOLD_BOXES) + split_item)
            file.write(str(value.NUM_LEFT_ITEMS) + split_item)
            file.write(str(value.NUM_LEFT_BOXES) + split_item)
            file.write(str(value.NUM_ITEMS_PER_BOX) + split_item)
            file.write( '-1' ) # default container is none
            file.write( ' \n' )
        file.close()
    return
