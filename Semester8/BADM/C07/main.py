import sys
import time
from datetime import datetime
from btcpay_api_error import BTCPayAPIError
from btcpay_client import BTCPayClient

# --- EXAMPLE USAGE ---
if __name__ == "__main__":
    MY_STORE_ID = "7ts2sgoBY2WxJ624eAxLg1z9EWEXf9QhBFMRbWGNpw4F" 
    MY_API_KEY = "b99542a9747286a6aec397e1fbdbdbabe4398354"
    # MY_BTCPAY_URL = "https://your_btcpay_server.com"

    if MY_API_KEY == "YOUR_NEW_ACCESS_TOKEN_API_KEY" or MY_STORE_ID == "YOUR_STORE_ID_PLACEHOLDER":
        print("Error: Please set MY_STORE_ID and MY_API_KEY before running the example!")
        sys.exit(1)

    client = BTCPayClient(api_key=MY_API_KEY, store_id=MY_STORE_ID)
    # client = BTCPayClient(api_key=MY_API_KEY, store_id=MY_STORE_ID, btcpay_url=MY_BTCPAY_URL)

    # --- Workflow Example ---
    try:
        print("\n*** STEP 0: Store Information ***")
        store_info = client.get_store_info()
        if store_info:
             print(f"Store Name: {store_info.get('name')}")
             print(f"Default Currency: {store_info.get('defaultCurrency')}")
        else:
             print("Could not fetch store information.")

        print("\n*** STEP 1: Create Invoice ***")
        invoice_amount_sats = 100 # SATS
        invoice_info = client.create_invoice(amount=invoice_amount_sats, currency="SATS", description="BDM Client", order_id="bdm-client-001")
        if not invoice_info or "id" not in invoice_info:
             print("Failed to create invoice.")
             sys.exit(1)

        invoice_id = invoice_info.get("id")
        checkout_link = invoice_info.get("checkoutLink")

        # 2. Get and Decode LN Invoice (with retry)
        print("\n*** STEP 2: Get and Decode LN Invoice ***")
        bolt11_invoice_str = None
        for attempt in range(3):
            bolt11_invoice_str = client.get_lightning_invoice(invoice_id)
            if bolt11_invoice_str:
                break
            print(f"LN invoice not available yet, waiting 2 seconds (attempt {attempt + 1}/3)")
            time.sleep(2)

        if bolt11_invoice_str:
            client.decode_bolt11(bolt11_invoice_str)
        else:
            print("Failed to retrieve LN invoice even after retries.")

        # 3. Check Initial Status
        print("\n*** STEP 3: Check Initial Status ***")
        initial_status = client.get_invoice_status(invoice_id)
        print(f"Initial status: {initial_status}")

        # 4. Simulate Manual Payment and Check Final Status
        if initial_status == "New" and checkout_link:
            print("\n--------------------------------------------------------------------")
            print(">>> USER ACTION REQUIRED: Please pay the invoice now.")
            print(f">>> Use the checkout link: {checkout_link}")
            print(">>> Open the link, get the LN invoice (lntb...)")
            print(">>> and pay it using a testnet Lightning wallet (e.g., Electrum).")
            input(">>> Press Enter after the invoice is paid (or if you want to skip)...")
            print("--------------------------------------------------------------------")

            print("\n*** STEP 4: Check Status After (Assumed) Payment ***")
            print("Waiting 3 seconds for server processing...")
            time.sleep(3)

            final_status = client.get_invoice_status(invoice_id)
            print(f"Checked status: {final_status}")
            if client.check_invoice_paid(invoice_id):
                 print(">>> Result: Invoice is paid (Settled).")
            else:
                 print(f">>> Result: Invoice is NOT paid (Status: {final_status}).")
        elif not checkout_link:
             print("\nCheckout link missing, cannot prompt for manual payment.")
        else:
            print(f"\nInvoice not in 'New' state (status: {initial_status}), skipping manual payment step.")

        # 5. List Recent Invoices
        print("\n*** STEP 5: List Last 5 Invoices ***")
        # Example: List recent invoices that are New or Settled
        # recent_invoices = client.list_invoices(statuses=['New', 'Settled'], limit=5)
        recent_invoices = client.list_invoices(limit=5)
        if recent_invoices:
             print(f"Found {len(recent_invoices)} invoices:")
             for inv in recent_invoices:
                 inv_id = inv.get('id', 'N/A')
                 inv_status = inv.get('status', 'N/A')
                 inv_amount = inv.get('amount', 'N/A')
                 inv_currency = inv.get('currency', '')
                 print(f"  - ID: {inv_id}, Status: {inv_status}, Amount: {inv_amount} {inv_currency}")
        else:
             print("Could not retrieve recent invoices.")


    except BTCPayAPIError as e:
        print(f"\nAn API error occurred: {e}")
    except Exception as e:
        print(f"\nAn unexpected general error occurred: {e}")

    print("\n--- Example script finished ---")