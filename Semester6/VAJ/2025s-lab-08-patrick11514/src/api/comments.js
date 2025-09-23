const comments = {
  1: ["Pickle Rick!"],
};

export async function getComments(characterId) {
  await fakeNetwork();
  return comments[characterId] || [];
}

export async function addComment(characterId, comment) {
  await fakeNetwork();
  if (!comments[characterId]) {
    comments[characterId] = [];
  }
  return comments[characterId].unshift(comment);
}

function fakeNetwork() {
  return new Promise((res) => {
    setTimeout(res, Math.random() * 1600);
  });
}
