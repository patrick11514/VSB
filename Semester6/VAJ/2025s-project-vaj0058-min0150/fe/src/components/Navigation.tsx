import { Link, useLocation } from 'react-router-dom';
import Rocket from './Rocket';
import Icon from './Icon';
import { paths } from './paths';

export default function Navigation() {
    const location = useLocation();

    return (
        <nav className="font-poppins m-2 flex items-center justify-between p-4 font-bold lg:px-[10%]">
            <h1 className="group font-ephesis cursor-pointer text-3xl 2xl:text-4xl">
                <Rocket /> StarTech
            </h1>
            <div className="flex items-center gap-4 font-bold">
                {paths.map((path) => {
                    const isActive = location.pathname === `/${path.path}`;
                    return (
                        <Link
                            to={path.path}
                            key={path.name}
                            className={
                                isActive
                                    ? 'border-b-text border-b-2'
                                    : 'after:border-b-text after:block after:origin-bottom-left after:scale-x-0 after:border-b-2 after:transition-transform hover:after:scale-x-100'
                            }
                        >
                            <Icon name={path.icon} /> {path.name}
                        </Link>
                    );
                })}
            </div>
        </nav>
    );
}
