
import multiprocessing 
import com 

if __name__ == "__main__":

    operation = int(input("Master or slave operation(1:Master/0:Slave): "))
    if (operation):
        #master operation 
        option = int(input(""" 
                 
                1. Seleccionar zonas
                2. Enviar con zonas por defecto ([0,0,0,0])
                3. Salir 

                """))
        if (option == 1):
            # select zones 
            z1 = int(input("zona 1[1-6]: "))
            z2 = int(input("zona 2[1-6]: "))
            z3 = int(input("zona 3[1-6]: "))
            z4 = int(input("zona 4[1-6]: "))

        elif (option == 2):
            z1,z2,z3,z4 = 0,0,0,0
        else:
            exit()
        
        message = int(input("Mensage: "))

        send_master_frame(message,[z1,z2,z3,z4])

        



