// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

contract Auction {
    enum State {
        Bidding,
        WaitingPassphrase,
        Completed
    }

    address private owner;
    string private name;
    string private description;
    bytes32 private originalFileHash;
    bytes32 private hashedFileHash;
    string private ipfsHandle;
    bytes32 private passphraseHash;
    string private revealedPassphrase;
    uint256 private endAt;

    address private maxBidder;
    uint256 private maxBid;
    State private state;

    mapping(address => uint256) private bidders;

    modifier onlyOwner() {
        require(msg.sender == owner, "Only owner");
        _;
    }

    modifier onlyWinner() {
        require(msg.sender == maxBidder, "Only winner");
        _;
    }

    modifier auctionEnded() {
        require(block.timestamp >= endAt, "Auction not ended");
        _;
    }

    modifier inState(State expectedState) {
        require(state == expectedState, "Invalid state");
        _;
    }

    constructor(
        uint256 _minBid,
        string memory _name,
        string memory _description,
        bytes32 _originalFileHash,
        bytes32 _hashedFileHash,
        string memory _ipfsHandle,
        bytes32 _passphraseHash,
        uint256 _endAt
    ) payable {
        owner = msg.sender;
        name = _name;
        description = _description;
        originalFileHash = _originalFileHash;
        hashedFileHash = _hashedFileHash;
        ipfsHandle = _ipfsHandle;
        passphraseHash = _passphraseHash;
        endAt = _endAt;
        state = State.Bidding;
        maxBid = _minBid;
    }

    function isOwner() external view returns (bool) {
        return msg.sender == owner;
    }

    function isWinner() external view returns (bool) {
        return msg.sender == maxBidder && maxBidder != address(0);
    }

    function canRefund() external view returns (uint256) {
        if (msg.sender == maxBidder) {
            return 0;
        }

        return bidders[msg.sender];
    }

    function canEndAuction() external view returns (bool) {
        return msg.sender == owner && state == State.Bidding;
    }

    function checkStatus() public view 
        returns (string memory, string memory, address, uint256, uint256, uint8, string memory, bytes32) 
    {
        uint8 derivedState = uint8(state);

        if (block.timestamp >= endAt && state == State.Bidding) {
            derivedState = uint8(State.WaitingPassphrase);
        }

        return (name, description, owner, endAt, maxBid, derivedState, ipfsHandle, hashedFileHash);
    }

    function bid() external payable inState(State.Bidding) {
        require(block.timestamp < endAt, "Bidding ended");
        require(msg.value > maxBid, "Bid too low");

        if (bidders[msg.sender] > 0) {
            bidders[msg.sender] += msg.value;
        } else {
            bidders[msg.sender] = msg.value;
        }

        uint256 bidderTotal = bidders[msg.sender];

        if (bidderTotal <= maxBid) {
            revert("Total bid not high enough");
        }

        maxBid = bidderTotal;
        maxBidder = msg.sender;
    }

    function endAuction() external onlyOwner inState(State.Bidding) {
        endAt = block.timestamp;
    }

    function revealPassphrase(string memory passphrase) external 
        onlyOwner 
        auctionEnded 
        inState(State.Bidding) 
    {
        require(sha256(bytes(passphrase)) == passphraseHash, "Invalid passphrase");

        revealedPassphrase = passphrase;
        state = State.Completed;

        (bool sent, ) = payable(owner).call{value: maxBid}("");
        require(sent, "Payout failed");
    }

    function givePassphrase() external view 
        onlyWinner 
        inState(State.Completed) 
        returns (string memory, string memory, bytes32, bytes32) 
    {
        return (ipfsHandle, revealedPassphrase, originalFileHash, passphraseHash);
    }

    function refund() external
     {
        require(msg.sender != maxBidder, "Winner cannot refund");

        uint256 amount = bidders[msg.sender];
        require(amount > 0, "No funds to refund");

        bidders[msg.sender] = 0;

        (bool sent, ) = payable(msg.sender).call{value: amount}("");
        require(sent, "Refund failed");
    }
}
