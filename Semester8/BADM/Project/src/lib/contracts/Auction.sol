// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

contract Auction {
    enum State { Bidding, WaitingPassphrase, Completed }

    address public owner;
    string public name;
    string public description;
    string public originalFileHash; // sha256 hex
    string public ipfsHandle;
    string public passphraseHash; // sha256 hex
    uint256 public endAt;

    address public maxBidder;
    uint256 public maxBid;
    State public state;

    mapping(address => uint256) public bidders;

    constructor(
        uint256 _minBid,
        string memory _name,
        string memory _description,
        string memory _originalFileHash,
        string memory _ipfsHandle,
        string memory _passphraseHash,
        uint256 _endAt
    ) payable {
        owner = msg.sender;
        name = _name;
        description = _description;
        originalFileHash = _originalFileHash;
        ipfsHandle = _ipfsHandle;
        passphraseHash = _passphraseHash;
        endAt = _endAt;
        state = State.Bidding;
        maxBid = _minBid;
    }

    function checkStatus() public {
        if (block.timestamp >= endAt && state == State.Bidding) {
            state = State.WaitingPassphrase;
        }
    }

    function bid() external payable {
        require(block.timestamp < endAt, "Bidding ended");
        require(state == State.Bidding, "Not in bidding state");
        require(msg.value > maxBid, "Bid too low");

        // previous max bidder becomes refundable
        if (maxBidder != address(0)) {
            bidders[maxBidder] += maxBid;
        }

        maxBid = msg.value;
        maxBidder = msg.sender;
    }

    function enterPassphrase(string memory passphrase) external {
        require(msg.sender == owner, "Only owner");
        require(block.timestamp >= endAt, "Too early");
        require(state == State.WaitingPassphrase, "Wrong state");

        // verify passphrase hash off-chain or via helper (placeholder)
        // For simplicity, we assume owner provides correct passphrase
        state = State.Completed;
        // transfer funds to owner
        payable(owner).transfer(maxBid);
    }

    function givePassphrase() external view returns (string memory, string memory) {
        require(state == State.Completed, "Not completed");
        require(msg.sender == maxBidder, "Only winner");
        return (passphraseHash, ipfsHandle);
    }

    function refund() external {
        require(state == State.Completed, "Not completed");
        require(msg.sender != maxBidder, "Winner cannot refund");
        uint256 amount = bidders[msg.sender];
        require(amount > 0, "No funds to refund");
        bidders[msg.sender] = 0;
        payable(msg.sender).transfer(amount);
    }
}
