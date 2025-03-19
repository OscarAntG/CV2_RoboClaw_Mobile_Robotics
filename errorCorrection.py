class ErrorByte:
    def __init__(self):
        super().__init__()
        self.motorDistanceMax = 32

    def serial_byte_correction(self, error_percent):
        error_percent = error_percent
        if error_percent > 1.0:
            corrected_error = error_percent - 1.0
            byte_send = int(corrected_error * self.motorDistanceMax)
        else:
            corrected_error = abs(error_percent)
            byte_send = int(corrected_error * self.motorDistanceMax)
        return byte_send
