class BTCPayAPIError(Exception):
    """Custom exception for BTCPay API errors."""
    def __init__(self, message, status_code=None, response_text=None):
        super().__init__(message)
        self.status_code = status_code
        self.response_text = response_text

    def __str__(self):
        # Provides more context in the error message
        if self.status_code:
            return f"BTCPayAPIError {self.status_code}: {super().__str__()} | Response: {self.response_text}"
        else:
            return f"BTCPayAPIError: {super().__str__()}"