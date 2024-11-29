import numpy as np
from sklearn import linear_model
from sklearn.linear_model import LassoCV
from sklearn.linear_model import RidgeCV
np.random.seed(12)
X = np.random.randn(80,100)
beta = np.zeros(100)
beta[:10] = 2
beta=beta.reshape(-1,1)
epsilon = np.random.randn(80,1)
Y=np.dot(X,beta)+epsilon
alphas = np.logspace(-3,1,50)
Ridges = RidgeCV(alphas=alphas,cv=5,fit_intercept=False)
Ridges.fit(X,np.ravel(Y))
BalphaR = Ridges.alpha_
print("Ridge回归最优参数:\n",BalphaR)
Rbest=linear_model.Ridge(alpha=BalphaR,fit_intercept=False).fit(X,Y)
Rcoefbest=Rbest.coef_
print("Ridge回归最优参数下的回归系数估计:\n",Rcoefbest)

Lassos = LassoCV(alphas=alphas,cv=5,fit_intercept=False)
Lassos.fit(X,np.ravel(Y))
best_alpha = Lassos.alpha_
print("Lasso回归最优参数:\n",best_alpha)
Lbest=linear_model.Lasso(alpha=best_alpha,fit_intercept=False).fit(X,Y)
Lcoefbest=Lbest.coef_
print("Lasso回归最优参数下的回归系数估计:\n",Lcoefbest)

from sklearn.model_selection import GridSearchCV
from asgl import Regressor
param_grid = {'lambda1': 10 ** np.arange(-2, 1.51, 0.1)}
alasso_model = Regressor(model='lm', penalization='alasso', weight_technique='lasso')
gscv = GridSearchCV(alasso_model, param_grid, scoring='neg_mean_squared_error', n_jobs=-1)
gscv.fit(X,np.ravel(Y))
final_model = gscv.best_estimator_
final_model.fit(X, np.ravel(Y))
final_model.coef_