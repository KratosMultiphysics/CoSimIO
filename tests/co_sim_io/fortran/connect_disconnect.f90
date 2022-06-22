program connect_disconnect

    USE COSIMIO_SUBS
    IMPLICIT NONE
    CHARACTER * 255            :: connection_n
    CHARACTER * 255            :: my_name
    CHARACTER * 255            :: connect_to
    
    my_name    = "c_connect_disconnect_a"
    connect_to = "c_connect_disconnect_b"
    
    
    CALL CONNECT(my_name, connect_to, connection_n)
    CALL DISCONNECT(TRIM(connection_n))
    
end program connect_disconnect

MODULE COSIMIO_SUBS
    IMPLICIT NONE
    
    CONTAINS
        SUBROUTINE CONNECT(my_name, connect_to, connection_n)
        
            USE co_sim_io
            IMPLICIT NONE
            
            TYPE(CoSimIO_Info)                         :: settings,connect_info
            CHARACTER * 255, INTENT(OUT)               :: connection_n
            CHARACTER * 255, INTENT(IN)                :: my_name
            CHARACTER * 255, INTENT(IN)                :: connect_to
            INTEGER                                    :: temp_int
            
            
            settings = CoSimIO_CreateInfo()
            CALL CoSimIO_Info_SetString(settings, "my_name", TRIM(my_name))
            CALL CoSimIO_Info_SetString(settings, "connect_to", TRIM(connect_to)) 
            CALL CoSimIO_Info_SetInt(settings, "echo_level", 4)
            
            
            
            connect_info = CoSimIO_Connect(settings)
             
            connection_n = CoSimIO_Info_GetString_Wrapper(connect_info, "connection_name") ! getting name of connection for future calls
            IF (CoSimIO_Info_GetInt(connect_info, "connection_status") /= CoSimIO_Connected) THEN
                WRITE(*,*) "connection not established "
        
            
            temp_int = CoSimIO_FreeInfo(settings) 
            temp_int = CoSimIO_FreeInfo(connect_info) 
            RETURN
        END SUBROUTINE CONNECT
        
        
        SUBROUTINE DISCONNECT(connection_n)
        
            USE co_sim_io            
            IMPLICIT NONE
            
            TYPE(CoSimIO_Info)                         :: disconnect_info, disconnect_settings
            CHARACTER * 255, INTENT(IN)                :: connection_n
            INTEGER                                    :: temp_int
            
            
            disconnect_settings = CoSimIO_CreateInfo()
            CALL CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_n) 
            disconnect_info = CoSimIO_Disconnect(disconnect_settings) 
            IF ( CoSimIO_Info_GetInt(disconnect_info, "connection_status") /= CoSimIO_Disconnected) THEN
                WRITE(*,*) "connection not disconnected"
            
            temp_int = CoSimIO_FreeInfo(disconnect_settings) 
            temp_int = CoSimIO_FreeInfo(disconnect_info) 
            RETURN
        END SUBROUTINE DISCONNECT
            
END MODULE COSIMIO_SUBS

        
        
        


