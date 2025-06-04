const Friend = require("../models/friendModel");

exports.sendFriendRequest = async (req, res) => {
  try {
    const { recipientId } = req.body;
    const requesterId = req.user.id;

    const existingRequest = await Friend.findOne({
      requester: requesterId,
      recipient: recipientId
    });

    if (existingRequest) {
      return res.status(400).json({ message: "Request already exists" });
    }

    const request = await Friend.create({
      requester: requesterId,
      recipient: recipientId,
      status: "pending"
    });

    res.status(201).json(request);
  } catch (err) {
    res.status(500).json({ message: err.message });
  }
};

exports.acceptFriendRequest = async (req, res) => {
  try {
    const { requestId } = req.body;

    const request = await Friend.findById(requestId);
    if (!request || request.status !== "pending") {
      return res.status(404).json({ message: "Friend request not found" });
    }

    request.status = "accepted";
    await request.save();

    // create reciprocal record
    await Friend.create({
      requester: request.recipient,
      recipient: request.requester,
      status: "accepted"
    });

    res.status(200).json({ message: "Friend request accepted" });
  } catch (err) {
    res.status(500).json({ message: err.message });
  }
};

exports.getMutualFriends = async (req, res) => {
  try {
    const currentUserId = req.user.id;

    const sent = await Friend.find({ requester: currentUserId, status: "accepted" }).select("recipient");
    const received = await Friend.find({ recipient: currentUserId, status: "accepted" }).select("requester");

    const sentIds = sent.map(f => f.recipient.toString());
    const receivedIds = received.map(f => f.requester.toString());

    const mutualIds = sentIds.filter(id => receivedIds.includes(id));

    res.status(200).json({ friends: mutualIds });
  } catch (err) {
    res.status(500).json({ message: err.message });
  }
};
