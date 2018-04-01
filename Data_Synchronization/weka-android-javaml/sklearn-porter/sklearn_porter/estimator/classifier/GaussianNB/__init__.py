# -*- coding: utf-8 -*-

from sklearn_porter.estimator.classifier.Classifier import Classifier


class GaussianNB(Classifier):
    """
    See also
    --------
    sklearn.naive_bayes.BernoulliNB

    http://scikit-learn.org/stable/modules/generated/sklearn.naive_bayes.BernoulliNB.html#sklearn.naive_bayes.BernoulliNB
    """

    SUPPORTED_METHODS = ['predict']

    # @formatter:off
    TEMPLATES = {
        'java': {
            'type':     '{0}',
            'arr':      '{{{0}}}',
            'arr[]':    '{type}[] {name} = {{{values}}};',
            'arr[][]':  '{type}[][] {name} = {{{values}}};',
            'indent':   '    ',
        },
        'js': {
            'type':     '{0}',
            'arr':      '[{0}]',
            'arr[]':    'var {name} = [{values}];',
            'arr[][]':  'var {name} = [{values}];',
            'indent':   '    ',
        },
    }
    # @formatter:on

    def __init__(self, estimator, target_language='java',
                 target_method='predict', **kwargs):
        """
        Port a trained estimator to the syntax of a chosen programming language.

        Parameters
        ----------
        :param estimator : AdaBoostClassifier
            An instance of a trained GaussianNB estimator.
        :param target_language : string
            The target programming language.
        :param target_method : string
            The target method of the estimator.
        """
        super(GaussianNB, self).__init__(
            estimator, target_language=target_language,
            target_method=target_method, **kwargs)
        self.estimator = estimator

    def export(self, class_name, method_name, **kwargs):
        """
        Port a trained estimator to the syntax of a chosen programming language.

        Parameters
        ----------
        :param class_name: string
            The name of the class in the returned result.
        :param method_name: string
            The name of the method in the returned result.

        Returns
        -------
        :return : string
            The transpiled algorithm with the defined placeholders.
        """

        # Arguments:
        self.class_name = class_name
        self.method_name = method_name

        # Estimator:
        est = self.estimator

        self.n_features = len(est.sigma_[0])
        self.n_classes = len(est.classes_)

        temp_type = self.temp('type')
        temp_arr = self.temp('arr')
        temp_arr_ = self.temp('arr[]')
        temp_arr__ = self.temp('arr[][]')

        # Create class prior probabilities:
        priors = [temp_type.format(self.repr(c)) for c in est.class_prior_]
        priors = ', '.join(priors)
        self.priors = temp_arr_.format(type='double', name='priors',
                                       values=priors)

        # Create sigmas:
        sigmas = []
        for sigma in est.sigma_:
            tmp = [temp_type.format(self.repr(s)) for s in sigma]
            tmp = temp_arr.format(', '.join(tmp))
            sigmas.append(tmp)
        sigmas = ', '.join(sigmas)
        self.sigmas = temp_arr__.format(type='double', name='sigmas',
                                        values=sigmas)

        # Create thetas:
        thetas = []
        for theta in est.theta_:
            tmp = [temp_type.format(self.repr(t)) for t in theta]
            tmp = temp_arr.format(', '.join(tmp))
            thetas.append(tmp)
        thetas = ', '.join(thetas)
        self.thetas = temp_arr__.format(type='double', name='thetas',
                                        values=thetas)

        if self.target_method == 'predict':
            return self.predict()

    def predict(self):
        """
        Transpile the predict method.

        Returns
        -------
        :return : string
            The transpiled predict method as string.
        """
        return self.create_class(self.create_method())

    def create_method(self):
        """
        Build the estimator method or function.

        Returns
        -------
        :return out : string
            The built method as string.
        """
        temp_method = self.temp('method.predict', n_indents=1, skipping=True)
        out = temp_method.format(**self.__dict__)
        return out

    def create_class(self, method):
        """
        Build the estimator class.

        Returns
        -------
        :return out : string
            The built class as string.
        """
        self.__dict__.update(dict(method=method))
        temp_class = self.temp('class')
        out = temp_class.format(**self.__dict__)
        return out
