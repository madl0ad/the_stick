unsigned char process_button( unsigned int hold, unsigned int state){
	if ( hold > S_DELAY ){	// next serie
			if ( state == NOT_PRESSED )
			{
			if ( ++mode > MODE_NUM ) { 
				mode=1; 
			}
			eeprom_write_byte(&e_mode,   mode);//Запись

			return 1;
			}
	}  else if ( hold > INC_DELAY ){	// next mode
		if ( state == NOT_PRESSED ){
			if (ss==1){
				ss=0;
				s=1;
			} else 	
				if ( ++s >= S_NUM+1 ){
				s=1;
				ss=1;
			} 	
			
			eeprom_write_byte(&e_mode,   mode);//Запись
			eeprom_write_byte(&e_serie,   s);//Запись
			eeprom_write_byte(&e_s_serie,   ss);//Запись	
			
			return 1;
		}
	}
	return 0;
}

unsigned char check_button(){
	static unsigned char last_button_state=NOT_PRESSED;
	unsigned char res=0;
	
	button_state=NOT_PRESSED;
	
	if ( (PORTE_IN & (1<<5)) > 0 ) {
		_delay_ms(10);
		if ( (PORTE_IN & (1<<5)) > 0 ) button_state = PRESSED;
	}
	
	if ( button_state == PRESSED ){
		hold++;
	}
	
	if ( last_button_state == button_state ){
		res = process_button( hold, button_state );
	}
	
	if ( button_state == NOT_PRESSED && last_button_state == NOT_PRESSED ){
		hold=0;
	}
	
	last_button_state=button_state;

	//res=button_state;
	return res;
}