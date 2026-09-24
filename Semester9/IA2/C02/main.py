from time import time

import numpy as np
import matplotlib.pyplot as plt

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_auc_score

import skimage as ski

import joblib

def extract_feature_image(img, feature_type, feature_coord=None):
    """Extract the haar feature for the current image"""
    ii = ski.transform.integral_image(img)
    return ski.feature.haar_like_feature(
        ii,
        0,
        0,
        ii.shape[0],
        ii.shape[1],
        feature_type=feature_type,
        feature_coord=feature_coord,
    )

images = ski.data.lfw_subset()
# To speed up the example, extract the two types of features only
feature_types = ['type-2-x', 'type-2-y']

# Compute the result
t_start = time()
X = [extract_feature_image(img, feature_types) for img in images]
X = np.stack(X)
time_full_feature_comp = time() - t_start

# Label images (100 faces and 100 non-faces)
y = np.array([1] * 100 + [0] * 100)

X_train, X_test, y_train, y_test = train_test_split(
    X, y, train_size=150, random_state=0, stratify=y
)

# Extract all possible features
feature_coord, feature_type = ski.feature.haar_like_feature_coord(
    width=images.shape[2], height=images.shape[1], feature_type=feature_types
)

# Train a random forest classifier and assess its performance
clf = RandomForestClassifier(
    n_estimators=1000, max_depth=None, max_features=100, n_jobs=-1, random_state=0
)

t_start = time()
clf.fit(X_train, y_train)
time_full_train = time() - t_start
auc_full_features = roc_auc_score(y_test, clf.predict_proba(X_test)[:, 1])

# Sort features in order of importance and plot the six most significant
idx_sorted = np.argsort(clf.feature_importances_)[::-1]

fig, axes = plt.subplots(3, 2)
for idx, ax in enumerate(axes.ravel()):
    image = images[0]
    image = ski.feature.draw_haar_like_feature(
        image, 0, 0, images.shape[2], images.shape[1], [feature_coord[idx_sorted[idx]]]
    )
    ax.imshow(image)
    ax.set_xticks([])
    ax.set_yticks([])

_ = fig.suptitle('The most important features')

cdf_feature_importances = np.cumsum(clf.feature_importances_[idx_sorted])
cdf_feature_importances /= cdf_feature_importances[-1]  # divide by max value
sig_feature_count = np.count_nonzero(cdf_feature_importances < 0.7)
sig_feature_percent = round(sig_feature_count / len(cdf_feature_importances) * 100, 1)
print(
    f'{sig_feature_count} features, or {sig_feature_percent}%, '
    f'account for 70% of branch points in the random forest.'
)


joblib.dump(clf, "clf_face.joblib")

# Select the determined number of most informative features
feature_coord_sel = feature_coord[idx_sorted[:sig_feature_count]]
feature_type_sel = feature_type[idx_sorted[:sig_feature_count]]
# Note: it is also possible to select the features directly from the matrix X,
# but we would like to emphasize the usage of `feature_coord` and `feature_type`
# to recompute a subset of desired features.

# Compute the result
t_start = time()
X = [extract_feature_image(img, feature_type_sel, feature_coord_sel) for img in images]
X = np.stack(X)
time_subs_feature_comp = time() - t_start

y = np.array([1] * 100 + [0] * 100)
X_train, X_test, y_train, y_test = train_test_split(
    X, y, train_size=150, random_state=0, stratify=y
)

t_start = time()
clf.fit(X_train, y_train)
time_subs_train = time() - t_start

auc_subs_features = roc_auc_score(y_test, clf.predict_proba(X_test)[:, 1])

summary = (
    f'Computing the full feature set took '
    f'{time_full_feature_comp:.3f}s, '
    f'plus {time_full_train:.3f}s training, '
    f'for an AUC of {auc_full_features:.2f}. '
    f'Computing the restricted feature set took '
    f'{time_subs_feature_comp:.3f}s, plus {time_subs_train:.3f}s '
    f'training, for an AUC of {auc_subs_features:.2f}.'
)


print(summary)
plt.show()