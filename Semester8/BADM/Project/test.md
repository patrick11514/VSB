Update README to the project description:
"EtherZar" (Aka Etherum Bazar)

It will be etherum marketplace, where people will login with metamask and will currently run only on SepoliaETH (for school and testing purposes). For user to enter on page, he must connect MetaMask. Then the page will have its database of created items via page, but will be usable for basically our SmartContract on blockchain. So the main page will have search window / address part (the adress of smart contract) where you will:
A) search by name from our database
B) via specific address on block chain

Or there will be list of searched (or by default sorted from newest to oldest) auctions (who will bet more, after timestamp will get the digital content).

The items sold here will be digital content uploaded on IPFS (Interplanetary File System). It will work like this:

- The user will select that he want to create Auction, It the form will contain specific inputs: "Name", "End Timestamp", "Min bid", "Description via MarkDown" and "key (for symetric hashing)" + "file to upload via input type file"

It will work like this:
User enter required name, timestamp and optional min bid + description in mark down, then he can use switch to select between two modes for the file upload:
A) Page will take user uploaded file, and upload it to IPFS for him, and takes handler of that file
B) The user will provide the file handle for us. This will be used for bigger files, because uploading 4GB file is not convinient, and because 90% of app will be clientside (hashing etc..) its not convinient

Then user will enter key for symetric hash of file also he can provide some own "passphrase" or hit button next to it to generate random one... (the random one is only available for the combination, where user will provide the raw file, not file handle. When filehandle provided, user must enter passphrase used to symetrically hash the file already uploaded to IPFS)

The workflow will then be like this:

1. We create sha256sum hash of the original file, and sha256 hash of passphrase
2. We hash input file with the passphrase (symetrically). Then we upload the file to IPFS and got the handle for it.
3. Smart contract creation, the smart contract will be submited to SepoliaETH blockchain with this input (constructor) values (min bid: number, name: string, description: string, original_file_hash: string, ipfs_handle: string, passphrase_hash: string, end_at_ts: number)
4. Now the app will wait for user in metamask to accept it, and wait for contract to appear on block chain
5. We now store the Name + description + contract addresss (from blockchain) into our database
6. We store in local storage of user the symetric key (? probably) so he can then autofill it? It should be still passphrase
7. We redirect user to /auction/contract_address

Smart contract description:
Smart contract will hold all data as private:
to the mentioned data in constructor also:

- Owner (its address)
- Max bidder (address)
- Max bid (number)
- State (Enum: Bidding, WaitingPassphrase, Completed)
- Bidders: Record<address, number> (address + total value of that address bidded)
- Passphrase: string? (by default empty)

And have functions:

- Check status (return the, max bid, timestamp, name, description, owner?) (public) (if now >= end_timestamp will also update state to WaitingPassphrase)
- Bid (payable) user can bid (Can be called if now < end_timestamp && state == Bidding)
- Enter Passphrase (only owner can call and can call only if now >= end_timestamp && state == WaitingPassphrase). The user will prove passphrase which will be checked with hash, if its okay:

1. We change state to Completed
2. We pay owner the sum of max_bid

- Give Passphrase (only max_bidder can call it and if now >= end_timestamp && state == Completed) and he will get back the raw passphrase + handle
- Refund (only callable if now >= end_timestamp && state == Completed and if address of caller is in Bidders Record + value > 0) then this call will 1. update Record[bidder_address] to 0 and transfer his bid back. (NOTE: The highest bidder can't be in this record, otherwise he will be able to refund money, which already owner of auction got, or we add check (if address != max_bidder))

The UI:

- Bidder can bid directly via site and via MetaMask popup. He will enter how much he want to bid, and then confirm transaction in MetaMask
- Owner can enter passphrase via site
- Bidders can then get refund via site
- The biggest bidder, can then get passphrase via button click, the page will then download file from IPFS, decrypt in browser and add button for user to download the decrypted file. It will also check the sha256 of original file. Or the user can download the file by himself, and decrypt it by itself outside app. Then the page will only display handle, passphrase and the sha256 of original file, so user can check if its correct. (Because we can't be sure that the file will be decrypted correctly in browser, because of some bugs or something, so we give user option to do it by himself)
