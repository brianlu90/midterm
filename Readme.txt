(1) D5, D6, D8 is for Confirm, up, down, button respectively.
    D13 is for analog output(square wave).
    A0 is for analog input(sampling).
    D1, D0, D2 is for serial Tx, serial Rx, and reset pin of uLCD respectively.
(2) After running the C++ code, the uLCD will show the current selection.
    Press the Confirm button and the uLCD will update the new selection.
    The program will start sampling the wave automatically.
(3) Run the Python code FFT.py to get the sampled results. The results, which
    include the sampled waveform and the FFT of the waveform, will be shown 
    in the graph after a while.